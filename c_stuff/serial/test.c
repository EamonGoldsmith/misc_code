#include <stdio.h>
#include <unistd.h> // for sleep
#include <string.h> // memcpy

#include "serial.h"

/*
	test program for serial port library
	send, recv, set custom baud rate
*/

comport_t port;

#define BOOL int
#define TRUE 1
#define FALSE 0

typedef unsigned char byte;

BOOL command(const char *cmd, float *first, float *second)
{
	serial_flush(port);

	// send command
	serial_cputs(port, cmd);
	serial_send_byte(port, '\n');

	// recv results
	byte buffer[10] = {0};
	int n = 0;
	int acc = 0;

	for (;;) {
		n = serial_poll(port, buffer, 10 - acc);

		if (n < 0) { printf("serial error\n"); return FALSE; }
		else { acc += n; }

		if (acc == 8) break;	
	}

	// print recv'd buffer
	printf("%s: ", cmd);
	for (int i = 0; i < 10; i++) { printf("%.2x ", buffer[i]); }
	printf("\n");

	// check control characters
	if (buffer[0] != 0x0a) { printf("missing stx\n"); return FALSE; }
	if (buffer[9] != 0) { printf("missing etx\n"); return FALSE; }

	// construct floats
	*first = 0.0f; memcpy(first, buffer + 1, sizeof(float));
	*second = 0.0f; memcpy(second, buffer + 5, sizeof(float));
	return TRUE;
}

void report_vibration()
{
	float rms = 0.0f, peak = 0.0f;

	if (!command("VB", &rms, &peak)) {
		printf("failed to read vibration\n");
	} else {
		printf("vibration\nrms: %f\npeak: %f\n\n", rms, peak);
	}
}

void report_temperature()
{
	float avg = 0.0f, peak = 0.0f;

	if (!command("TP", &avg, &peak)) {
		printf("failed to read temperature\n");
	} else {
		printf("temperature\naverage: %f\npeak: %f\n\n", avg, peak);
	}
}

void report_sensor()
{

	for (int i = 1; i <= 6; i++)
	{
		char cmd[4];
		sprintf(cmd, "CH%i", i);
		
		float avg = 0.0f, peak = 0.0f;

		if (command(cmd, &avg, &peak))
		{
			printf("sensor\naverage: %f\npeak: %f\n\n", avg, peak);
		}
		else
		{
			printf("failed to read sensor\n");
			break;
		}
	}
}

const int ONE_SECOND = 1000000;

int main()
{
	port = serial_open("COM4", 115200, "8N1", 0);
	
	if (port < 0) {
		fprintf(stderr, "failed to open serial port\n");
	}

	while (1)
	{
		report_temperature();
		sleep(1);

		report_vibration();
		sleep(1);
		
		report_sensor();
		sleep(1);
	}

	serial_close(port);
	return 0;
}
