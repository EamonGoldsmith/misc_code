/*
 * mcp2515_def.h
 *
 *  Created on: Jun 6, 2024
 *      Author: Eamon
 */

#ifndef INC_MCP2515_DEF_H_
#define INC_MCP2515_DEF_H_

enum error_mode {
	mcp_error,		// not too bad maybe try again
	mcp_ok,			// all good
	mcp_critical	// very wrong, restart device
};

enum operation_mode {
	debug,
	standard
};

#define TIMEOUT 1000

#define SIDH 0
#define SIDL 1
#define EID8 2
#define EID0 3

#define TXB_EXIDE_M 0x08
#define DLC_MASK 0x0F
#define RTR_MASK 0x40

#define RXB_RX_ANY 0x60
#define RXB_RX_EXT 0x40
#define RXB_RX_STD 0x20
#define RXB_RX_STDEXT 0x00
#define RXB_RX_MASK 0x60
#define RXB_BUKT_MASK (1<<2)

#define NUM_TX_BUFFERS (3)

// Bits in the TXBnCTRL registers.
#define TXB_TXBUFE 0x80
#define TXB_ABTF 0x40
#define TXB_MLOA 0x20
#define TXB_TXERR 0x10
#define TXB_TXREQ 0x08
#define TXB_TXIE 0x04
#define TXB_TXP10 0x03

#define TXB_RTR 0x40
#define RXB_IDE 0x08
#define RXB_RTR 0x40

#define STAT_RXIF_MASK (0x03)
#define STAT_RX0IF (1<<0)
#define STAT_RX1IF (1<<1)

#define EFLG_RX1OVR (1<<7)
#define EFLG_RX0OVR (1<<6)
#define EFLG_TXBO (1<<5)
#define EFLG_TXEP (1<<4)
#define EFLG_RXEP (1<<3)
#define EFLG_TXWAR (1<<2)
#define EFLG_RXWAR (1<<1)
#define EFLG_EWARN (1<<0)
#define EFLG_ERRORMASK (0xF8)

// Define MCP2515 register addresses
#define RXF0SIDH 0x00
#define RXF0SIDL 0x01
#define RXF0EID8 0x02
#define RXF0EID0 0x03
#define RXF1SIDH 0x04
#define RXF1SIDL 0x05
#define RXF1EID8 0x06
#define RXF1EID0 0x07
#define RXF2SIDH 0x08
#define RXF2SIDL 0x09
#define RXF2EID8 0x0A
#define RXF2EID0 0x0B
#define CANSTAT 0x0E
#define CANCTRL 0x0F
#define RXF3SIDH 0x10
#define RXF3SIDL 0x11
#define RXF3EID8 0x12
#define RXF3EID0 0x13
#define RXF4SIDH 0x14
#define RXF4SIDL 0x15
#define RXF4EID8 0x16
#define RXF4EID0 0x17
#define RXF5SIDH 0x18
#define RXF5SIDL 0x19
#define RXF5EID8 0x1A
#define RXF5EID0 0x1B
#define TEC 0x1C
#define REC 0x1D
#define RXM0SIDH 0x20
#define RXM0SIDL 0x21
#define RXM0EID8 0x22
#define RXM0EID0 0x23
#define RXM1SIDH 0x24
#define RXM1SIDL 0x25
#define RXM1EID8 0x26
#define RXM1EID0 0x27
#define CNF3 0x28
#define CNF2 0x29
#define CNF1 0x2A
#define CANINTE 0x2B
#define CANINTF 0x2C
#define EFLG 0x2D
#define TXB0CTRL 0x30
#define TXB1CTRL 0x40
#define TXB2CTRL 0x50
#define RXB0CTRL 0x60
#define RXB0SIDH 0x61
#define RXB1CTRL 0x70
#define RXB1SIDH 0x71
#define TX_INT 0x1C
#define TX01_INT 0x0C
#define RX_INT 0x03
#define NO_INT 0x00

#define TX01_MASK 0x14
#define TX_MASK 0x54

// Define SPI Instruction Set
#define SPI_WRITE 0x02
#define SPI_READ 0x03
#define SPI_BITMOD 0x05
#define SPI_LOAD_TX0 0x40
#define SPI_LOAD_TX1 0x42
#define SPI_LOAD_TX2 0x44
#define SPI_RTS_TX0 0x81
#define SPI_RTS_TX1 0x82
#define SPI_RTS_TX2 0x84
#define SPI_RTS_ALL 0x87
#define SPI_READ_RX0 0x90
#define SPI_READ_RX1 0x94
#define SPI_READ_STATUS 0xA0
#define SPI_RX_STATUS 0xB0
#define SPI_RESET 0xC0

// CANCTRL Register Values
#define MODE_NORMAL 0x00
#define MODE_SLEEP 0x20
#define MODE_LOOPBACK 0x40
#define MODE_LISTENONLY 0x60
#define MODE_CONFIG 0x80
#define MODE_POWERUP 0xE0
#define MODE_MASK 0xE0
#define MODE_ONESHOT 0x08

#define ABORT_TX 0x10

#define CLKOUT_ENABLE 0x04
#define CLKOUT_DISABLE 0x00

#define CLKOUT_PS1 0x00
#define CLKOUT_PS2 0x01
#define CLKOUT_PS4 0x02
#define CLKOUT_PS8 0x03

// speeds
#define SPD_125K_CNF1 (0x03)
#define SPD_125K_CNF2 (0x86)
#define SPD_125K_CNF3 (0xF0)

#define SPD_250K_CNF1 (0x41)
#define SPD_250K_CNF2 (0xF1)
#define SPD_250K_CNF3 (0x85)

#define SPD_500K_CNF1 (0x00)
#define SPD_500K_CNF2 (0xF0)
#define SPD_500K_CNF3 (0x86)

// CNF1 Register Values
#define SJW1 0x00
#define SJW2 0x40
#define SJW3 0x80
#define SJW4 0xC0

//  CNF2 Register Values
#define BTLMODE 0x80
#define SAMPLE_1X 0x00
#define SAMPLE_3X 0x40

// CNF3 Register Values
#define SOF_ENABLE 0x80
#define SOF_DISABLE 0x00
#define WAKFIL_ENABLE 0x40
#define WAKFIL_DISABLE 0x00

// CANINTF Register Bits
#define RX0IF 0x01
#define RX1IF 0x02
#define TX0IF 0x04
#define TX1IF 0x08
#define TX2IF 0x10
#define ERRIF 0x20
#define WAKIF 0x40
#define MERRF 0x80

// vestigial :( one day we will have bit rate calculations
#define LIGHT_SPEED 299_792_458

#endif /* INC_MCP2515_DEF_H_ */
