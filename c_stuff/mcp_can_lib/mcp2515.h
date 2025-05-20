/*
 * mcp2515.h
 *
 *  Created on: Jun 6, 2024
 *      Author: Eamon
 */

#include "stm32f3xx_hal.h"

#ifndef INC_MCP2515_H_
#define INC_MCP2515_H_

enum can_speed {
	B125k, B250k, B500k
};

enum frame_type {
	data,
	remote,
	error,
	overload
};

struct can_frame {
	unsigned long id;
	int len;
	unsigned char data[8];
	enum frame_type type;
};

int init_mcp(SPI_HandleTypeDef *h_spi, enum can_speed speed);
int build_frame(char msg[8], struct can_frame *frame);
int recv_can(struct can_frame *msg);
int send_can(struct can_frame *msg);

// ISR
void can_handle_frame(struct can_frame *next_frame);

#endif /* INC_MCP2515_H_ */
