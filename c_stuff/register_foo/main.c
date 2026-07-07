#include <stdio.h>
#include <stdint.h>

struct reg_map_thing
{
	uint8_t ready : 1;
	uint8_t count : 7;
	uint8_t thing_upper : 4;
	uint8_t thing_lower : 4;
	uint8_t bla : 3;
};

uint16_t reg = 0x0000;

int main()
{
	// print whole register
	printf("reg: ");
	for (int i = 0; i < 16; i++) {
		printf("%c", reg & (0x1 << i) ? '1' : '0');
	}
	printf("\n");

	// cast to map
	struct reg_map_thing reg_manip;

	// modify
	reg_manip.ready = 0xff;
	reg_manip.thing_upper = 0xe;
	reg_manip.thing_lower = 0x1;

	reg = *((uint16_t*)&reg_manip);

	// print whole register
	printf("reg: ");
	for (int i = 0; i < 16; i++) {
		printf("%c", reg & (0x1 << i) ? '1' : '0');
	}
	printf("\n");

	printf("sizeof reg map: %li\n", sizeof(reg_manip));

	return 0;
}
