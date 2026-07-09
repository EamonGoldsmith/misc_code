#ifndef SERIAL_H_
#define SERIAL_H_

#include <stddef.h>

#ifdef _WIN32

#include <windows.h>

typedef char* comport_t;

#define INVALID_COMPORT INVALID_HANDLE_VALUE
#define IS_VALID_COMPORT(port) (port != INVALID_COMPORT)
#define GET_SERIAL_ERROR() GetLastError()

#else

#include <string.h>

typedef int comport_t;

#define INVALID_COMPORT (-1)
#define IS_VALID_COMPORT(port) (port > 0)
#define GET_SERIAL_ERROR() strerror(errno)

#endif

#define SERIAL_ERROR (-1)
#define SERIAL_OK (1)

comport_t serial_open(
	const char *devname, int baudrate, const char *mode, int flowctrl
);

int serial_poll(comport_t port, unsigned char *buf, int size);

void serial_flush(comport_t port);
int serial_send_byte(comport_t port, unsigned char byte);
int serial_send_buf(comport_t port, unsigned char *buf, int size);
void serial_close(comport_t port);

// blocking function!
void serial_break(comport_t port, int delay_ms);

// send string to port, must be null terminated string
int serial_cputs(comport_t port, const char *string);

#endif // SERIAL_H_
