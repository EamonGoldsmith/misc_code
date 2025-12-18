/*
 * mcp2515.c
 *
 *  Created on: Jun 6, 2024
 *      Author: Eamon
 */

/* todo:
	 - initialisation
	 - send
	 - recv
	 - set filters
	 - set masks

	to test:
	 - if message stays in buffer after send (preload)
*/

#include "mcp2515.h"
#include "mcp2515_def.h"

SPI_HandleTypeDef *_h_spi;

// timing operations
enum error_mode set_mode(unsigned char mode);
enum error_mode set_rate(enum can_speed speed);

// buffer operations
enum error_mode init_buffers();
enum error_mode get_free_tx_buffer(unsigned char *tx_buffer);
enum error_mode write_tx_buffer(struct can_frame *msg, unsigned char tx_buffer);
enum error_mode enable_recv_buffer(unsigned char mode);

enum error_mode check_rx_buffers();
enum error_mode read_rx_buffer(unsigned char rx_buffer, unsigned char *len,  unsigned char data[8]);
enum error_mode read_id(unsigned long *id, unsigned char *len, unsigned char data[8]);

// SPI operations
enum error_mode write_register(unsigned char reg, unsigned char data);
enum error_mode read_register(unsigned char reg, unsigned char *value);

enum error_mode modify_register(unsigned char reg, unsigned char mask, unsigned char data);
enum error_mode write_buffer_registers(unsigned char reg, unsigned char data[], int n);

int init_mcp(SPI_HandleTypeDef *h_spi, enum can_speed speed)
{
	if (h_spi->State != HAL_SPI_STATE_READY) {
		// error, should have started SPI by now
		return 0;
	}

	_h_spi = h_spi;

	// reset device (puts in configure anyway)
	unsigned char cmd = SPI_RESET;
	HAL_SPI_Transmit(h_spi, &cmd, 1, TIMEOUT);
	HAL_Delay(10);

	if (set_rate(speed) != mcp_ok) {
		return 0;
	}

	if (init_buffers() != mcp_ok) {
		return 0;
	}

	// set default chip interrupts
	if (write_register(CANINTE, RX0IF | RX1IF) != mcp_ok) {
		return 0;
	}

	if (set_mode(MODE_NORMAL) != mcp_ok) {
		return 0;
	}

	HAL_Delay(10);
	return 1;
}

int recv_can(struct can_frame *msg)
{

	return 0;
}

int send_can(struct can_frame *msg)
{
	// find a free buffer
	unsigned char tx_buffer = 0x00;
	if (get_free_tx_buffer(&tx_buffer) != mcp_ok) {
			return 0;
	}

	// transfer message data to buffer
	write_tx_buffer(msg, tx_buffer);
	modify_register(tx_buffer - 1, TXB_TXREQ, TXB_TXREQ);

	// check message was sent (didn't lose arbitration)
	unsigned char result;
	if (read_register(tx_buffer - 1, &result) != mcp_ok) {
		return 0;
	}

	if (result & TXB_TXREQ) {
		return 0;
	}

	return 1;
}

enum error_mode set_mode(unsigned char mode)
{
	modify_register(CANCTRL, MODE_MASK, mode);

	unsigned char chip_mode;
	if (read_register(CANCTRL, &chip_mode) != mcp_ok) {
		return mcp_error;
	}

	chip_mode &= MODE_MASK;
	if (chip_mode == mode) {
		return mcp_ok;
	}

	return mcp_error;
}

enum error_mode set_rate(enum can_speed speed)
{
	unsigned char set = 1, cfg1, cfg2, cfg3;

	// these are the only speeds you need :)
	switch (speed) {
	case (B125k):
		cfg1 = SPD_125K_CNF1;
		cfg2 = SPD_125K_CNF3;
		cfg3 = SPD_125K_CNF2;
		break;

	case (B250k):
		cfg1 = SPD_250K_CNF1;
		cfg2 = SPD_250K_CNF2;
		cfg3 = SPD_250K_CNF3;
		break;

	case (B500k):
		cfg1 = SPD_500K_CNF1;
		cfg2 = SPD_500K_CNF2;
		cfg3 = SPD_500K_CNF3;
		break;

	default:
		set = 0;
		break;
	}

	if(set) {
		write_register(CNF1, cfg1);
		write_register(CNF2, cfg2);
		write_register(CNF3, cfg3);
		return mcp_ok;
	} else {
		// invalid baud rate entered
		return mcp_error;
	}

	return mcp_ok;
}

enum error_mode init_buffers()
{
	// clear internal buffers
	unsigned char i, a1, a2, a3;

	a1 = TXB0CTRL;
	a2 = TXB1CTRL;
	a3 = TXB2CTRL;

	for (i = 0; i < 14; i++) {
		write_register(a1, 0);
		write_register(a2, 0);
		write_register(a3, 0);
		a1++;
		a2++;
		a3++;
	}

	write_register(RXB0CTRL, 0);
	write_register(RXB1CTRL, 0);

	return mcp_ok;
}

enum error_mode get_free_tx_buffer(unsigned char *tx_buffer)
{
	unsigned char ctrlval;
	unsigned char ctrlregs[NUM_TX_BUFFERS] = {TXB0CTRL, TXB1CTRL, TXB2CTRL};

	*tx_buffer = 0x00;

	for (int i = 0; i < NUM_TX_BUFFERS; i++) {
		 if (read_register(ctrlregs[i], &ctrlval) != mcp_ok) {
			 return mcp_error;
		 }

		 if ((ctrlval & TXB_TXREQ) == 0) {
			 *tx_buffer = ctrlregs[i] + 1;
			 return mcp_ok;
		 }
	}

	// no error occurred but also no buffers were free
	return mcp_ok;
}

enum error_mode write_tx_buffer(struct can_frame *msg, unsigned char tx_buffer)
{
	// write data
	write_buffer_registers(tx_buffer + 5, msg->data, msg->len);

	if (msg->type == remote) {
		msg->len |= RTR_MASK;
	}

	write_register(tx_buffer + 4, msg->len);
	write_register(tx_buffer, msg->id);

	return mcp_ok;
}

enum error_mode write_id_tx_buffer(unsigned char tx_buffer, unsigned char ext, unsigned long id)
{
	uint16_t can_id;
	unsigned char tx_buffer_data[4];

	can_id = (uint16_t)(id & 0xffff);

	if (ext == 1) {
		tx_buffer_data[EID0] = can_id & 0xff;
		tx_buffer_data[EID8] = can_id >> 8;
		can_id = (uint16_t)(id >> 16);
		tx_buffer_data[SIDL] = can_id & 0x03;
		tx_buffer_data[SIDL] = (can_id & 0x1c) << 3;
		tx_buffer_data[SIDL] |= TXB_EXIDE_M;
		tx_buffer_data[SIDH] = can_id >> 5;
	} else {
		tx_buffer_data[SIDH] = can_id >> 3;
		tx_buffer_data[SIDL] = (can_id & 0x07) << 5;
		tx_buffer_data[EID0] = 0;
		tx_buffer_data[EID8] = 0;
	}

	write_buffer_registers(tx_buffer, tx_buffer_data, 4);
	return mcp_ok;
}

enum error_mode enable_recv_buffer(unsigned char mode)
{
	if (mode == debug) {
		modify_register(RXB0CTRL,
		RXB_RX_MASK | RXB_BUKT_MASK,
		RXB_RX_ANY | RXB_BUKT_MASK);
		modify_register(RXB1CTRL, RXB_RX_MASK, RXB_RX_ANY);
	} else if (mode == standard) {
		modify_register(RXB0CTRL,
		RXB_RX_MASK | RXB_BUKT_MASK,
		RXB_RX_STDEXT | RXB_BUKT_MASK);
		modify_register(RXB1CTRL, RXB_RX_MASK, RXB_RX_STDEXT);
	}

	return mcp_ok;
}

enum error_mode write_register(unsigned char reg, unsigned char data)
{
	unsigned char cmd = SPI_WRITE;
	if (HAL_SPI_Transmit(_h_spi, &cmd, 1, TIMEOUT) != HAL_OK) {
		return mcp_error;
	}
	if (HAL_SPI_Transmit(_h_spi, &reg, 1, TIMEOUT) != HAL_OK) {
		return mcp_error;
	}
	if (HAL_SPI_Transmit(_h_spi, &data, 1, TIMEOUT) != HAL_OK) {
		return mcp_error;
	}

	return mcp_ok;
}

enum error_mode write_buffer_registers(unsigned char reg,
		unsigned char data[], int n)
{
	unsigned char cmd = SPI_WRITE;
	if (HAL_SPI_Transmit(_h_spi, &cmd, 1, TIMEOUT) != HAL_OK) {
		return mcp_error;
	}
	if (HAL_SPI_Transmit(_h_spi, &reg, 1, TIMEOUT) != HAL_OK) {
		return mcp_error;
	}

	if (HAL_SPI_Transmit(_h_spi, data, n, TIMEOUT) != HAL_OK) {
		return mcp_error;
	}

	return mcp_ok;
}

enum error_mode modify_register(unsigned char reg, unsigned char mask,
		unsigned char data)
{
	// Form: [modify command][address][bit mask][byte data]
	unsigned char cmd = SPI_BITMOD;
	HAL_SPI_Transmit(_h_spi, &cmd, 1, TIMEOUT);
	HAL_SPI_Transmit(_h_spi, &reg, 1, TIMEOUT);
	HAL_SPI_Transmit(_h_spi, &mask, 1, TIMEOUT);
	HAL_SPI_Transmit(_h_spi, &data, 1, TIMEOUT);

	return mcp_ok;
}

enum error_mode read_register(unsigned char reg, unsigned char *value)
{
	// Form: [read command][register address]
	unsigned char cmd = SPI_READ;
	if (HAL_SPI_Transmit(_h_spi, &cmd, 1, TIMEOUT) != HAL_OK) {
		return mcp_error;
	}
	if (HAL_SPI_Transmit(_h_spi, &reg, 1, TIMEOUT) != HAL_OK) {
		return mcp_error;
	}
	if (HAL_SPI_Receive(_h_spi, value, 1, TIMEOUT) != HAL_OK) {
		return mcp_error;
	}

	return mcp_ok;
}
