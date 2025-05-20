#include <stdio.h>
#include <cstdint>
#include <unistd.h>

#define NUM_PARAMS 3

enum param_type {
	TEMP,
	YANG,
	XANG
};

volatile uint8_t temp;
volatile uint8_t y_angle;
volatile uint8_t x_angle;

struct param_pair {
	enum param_type param;
	volatile uint8_t *param_data;
};

struct param_pair params_list[] = {
	{TEMP, &temp},
	{XANG, &x_angle},
	{YANG, &y_angle}
};

const char* convert_string(enum param_type pt) {
	switch (pt) {
	case TEMP:
		return "temp: ";
	case XANG:
		return "x angle: ";
	case YANG:
		return "y angle: ";
	default:
		return "param not found ";
	}
}

void print_param(enum param_type pt) {

	printf("printing %s%u\n", convert_string(pt), *(params_list[pt].param_data));
}

void process_msgs() {
	// show params
	print_param(TEMP);
	print_param(XANG);
	print_param(YANG);
}


int main()
{
	printf("starting...\n");

	// test loop
	while (1) {
		process_msgs();

		x_angle += 1;
		y_angle += 2;
		temp += 3;

		usleep(1000000);
	}

	return 0;
}
