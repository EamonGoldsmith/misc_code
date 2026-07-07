#ifndef SERIAL_H_
#define SERIAL_H_

#include <stddef.h>

// open devname, with settings given, return a port reference number
size_t serial_open(const char *devname, int baudrate, const char *mode,
	int flowctrl);

// take port, buffer and size, return number of bytes writen to buffer
int serial_poll(size_t port_num, unsigned char *buf, int size);

void serial_flush(size_t port_num);
int serial_send_byte(size_t port_num, unsigned char byte);
int serial_send_buf(size_t port_num, unsigned char *buf, int size);
void serial_close(size_t port_num);

// send string to port, must be null terminated string
void serial_cputs(size_t port_num, const char *text);

// send a break for delay_ms, blocking, will stop break after time finish.
void serial_break(size_t port_num, int delay_ms);

#endif // SERIAL_H_
