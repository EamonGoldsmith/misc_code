#include "serial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if _WIN32

comport_t serial_open(
	const char *devname, int baudrate, const char *mode, int flowctrl
) {
	char mode_str[512];
	sprintf(mode_str,
		"baud=%i data=%i parity=%c stop=%i xon=off to=off odsr=off dtr=on rts=%s",
baudrate, mode[0] - '0', mode[1], mode[2] - '0',
		flowctrl ? "on" : "off");

	printf("mode_str: %s\n", mode_str);

	// devname must have form "\\\\.\\COMX"
	comport_t port = CreateFileA(devname, 
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (port == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "unable to open port: %s\n", devname);
		return INVALID_COMPORT;
	}

	DCB port_settings;
	memset(&port_settings, 0, sizeof(port_settings));
	port_settings.DCBlength = sizeof(port_settings);

	if (!BuildCommDCBA(mode_str, &port_settings)) {
		fprintf(stderr, "failed to set port DCB settings\n");
		CloseHandle(port);
		return INVALID_COMPORT;
	}

	if (flowctrl) {
		port_settings.fOutxCtsFlow = TRUE;
		port_settings.fRtsControl = RTS_CONTROL_HANDSHAKE;
	}
 
	if (!SetCommState(port, &port_settings)) {
		fprintf(stderr, "failed to set port settings\n");
		CloseHandle(port);
		return INVALID_COMPORT;
	}

	COMMTIMEOUTS Cptimeouts;
	Cptimeouts.ReadIntervalTimeout = MAXDWORD;
	Cptimeouts.ReadTotalTimeoutMultiplier = 0;
	Cptimeouts.ReadTotalTimeoutConstant = 0;
	Cptimeouts.WriteTotalTimeoutMultiplier = 0;
	Cptimeouts.WriteTotalTimeoutConstant = 0;

	if (!SetCommTimeouts(port, &Cptimeouts)) {
		fprintf(stderr, "failed to set port timeout\n");
		CloseHandle(port);
		return INVALID_COMPORT;
	}

	return port;
}

int serial_poll(comport_t port, unsigned char *buf, int size)
{
	int n;

	if (!ReadFile(port, buf, size, (LPDWORD)(&n), NULL)) {
		return SERIAL_ERROR;
	}

	return n;
}

void serial_flush(comport_t port)
{
	PurgeComm(port, PURGE_RXCLEAR | PURGE_RXABORT);
	PurgeComm(port, PURGE_TXCLEAR | PURGE_TXABORT);
}

int serial_send_byte(comport_t port, unsigned char byte)
{
	int n;

	if (!WriteFile(port, &byte, 1, (LPDWORD)(&n), NULL)) {
		return SERIAL_ERROR;
	}

	return (n > -1 ? SERIAL_OK : SERIAL_ERROR);
}

int serial_send_buf(comport_t port, unsigned char *buf, int size)
{
	int n;

	if (!WriteFile(port, buf, size, (LPDWORD)(&n), NULL)) {
		return SERIAL_ERROR;
	}

	return (n > -1 ? SERIAL_OK : SERIAL_ERROR);
}

void serial_close(comport_t port)
{
	CloseHandle(port);
}

void serial_break(comport_t port, int delay_ms)
{
	if (!SetCommBreak(port)) {
		fprintf(stderr, "failed to set break\n");
	}

	Sleep(1);

	if (!ClearCommBreak(port)) {
		fprintf(stderr, "failed to clear break\n");
	}
}

#else
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <termios.h>

// man 3 termios
// man 4 tty_ioctl

comport_t serial_open(const char *devname, int baudrate, const char *mode, int flowctrl)
{
	comport_t port;
	int err, baudr, status;
	int cbits=CS8, cpar=0, ipar=IGNPAR, bstop=0;

	if (strlen(mode) != 3) {
		fprintf(stderr, "invalid mode");
		return INVALID_COMPORT;
	}

	// data bits
	switch (mode[0]) {
	case '8':
		cbits = CS8;
		break;
	case '7':
		cbits = CS7;
		break;
	case '6':
		cbits = CS6;
		break;
	case '5':
		cbits = CS5;
		break;
	default:
		fprintf(stderr, "invalid number of data-bits");
		return INVALID_COMPORT;
	}

	// partiy
	switch (mode[1]) {
	case 'N':
	case 'n':
		cpar = 0;
		ipar = IGNPAR;
		break;
	case 'E':
	case 'e':
		cpar = PARENB;
		ipar = INPCK;
		break;

	case 'O':
	case 'o':
		cpar = (PARENB | PARODD);
		ipar = INPCK;
		break;
	default:
		fprintf(stderr, "invalid parity");
		return INVALID_COMPORT;
	}

	// stop bits
	switch (mode[2]) {
	case '1':
		bstop = 0;
		break;
	case '2':
		bstop = CSTOPB;
		break;
	default:
		fprintf(stderr, "invalid number of stop bits");
		return INVALID_COMPORT;
	}

	port = open(devname, O_RDWR | O_NOCTTY | O_NDELAY);
	
	if (port == -1) {
		perror("unable to open comport");
		return INVALID_COMPORT;
	}

	// lock access so that another process can't also use the port
	if (flock(port, LOCK_EX | LOCK_NB) != 0) {
		close(port);
		perror("another process has locked the comport");
		return INVALID_COMPORT;
	}

	struct termios old_port_settings;
	struct termios new_port_settings;

	// backup port settings
	err = tcgetattr(port, &old_port_settings);
	if (err==-1) {
		close(port);
		flock(port, LOCK_UN); // free the port so that others can use it.
		perror("unable to read portsettings");
		return INVALID_COMPORT;
	}

	// clear struct
	memset(&new_port_settings, 0, sizeof(new_port_settings));

	// set params
	new_port_settings.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD;
	if (flowctrl) {
		new_port_settings.c_cflag |= CRTSCTS;
	}

	new_port_settings.c_iflag = ipar;
	new_port_settings.c_oflag = 0;
	new_port_settings.c_lflag = 0;
	new_port_settings.c_cc[VMIN] = 0; // block untill n bytes are received
	new_port_settings.c_cc[VTIME] = 0; // block untill a timer expires (n * 100 mSec.)

	// set baudrate (NON-STANDARD)
	new_port_settings.c_cflag &= ~(CBAUD | CBAUDEX);
	new_port_settings.c_cflag |= B38400;

	// set the temporary baudrate and connection settings
	err = tcsetattr(port, TCSANOW, &new_port_settings);
	if (err == -1) {
		tcsetattr(port, TCSANOW, &old_port_settings);
		close(port);
		flock(port, LOCK_UN);
		perror("unable to adjust portsettings");
		return INVALID_COMPORT;
	}

	// update with custom non-standard rate
	struct serial_struct serial;
	if (ioctl(port, TIOCGSERIAL, &serial) == -1) {
		tcsetattr(port, TCSANOW, &old_port_settings);
		flock(port, LOCK_UN);
		perror("unable to get portstatus");
		return INVALID_COMPORT;
	}
	
	serial.flags &= ~ASYNC_SPD_MASK;
	serial.flags |= ASYNC_SPD_CUST;
	serial.custom_divisor = serial.baud_base / baudrate;

	if (ioctl(port, TIOCSSERIAL, &serial) == -1) {
		tcsetattr(port, TCSANOW, &old_port_settings);
		flock(port, LOCK_UN);
		perror("unable to get portstatus");
		return INVALID_COMPORT;
	}

	// get status
	if (ioctl(port, TIOCMGET, &status) == -1) {
		tcsetattr(port, TCSANOW, &old_port_settings);
		flock(port, LOCK_UN);
		perror("unable to get portstatus");
		return INVALID_COMPORT;
	}

	// turn on DTR
	status |= TIOCM_DTR;
	// turn on RTS
	status |= TIOCM_RTS;

	// set modem bits
	if (ioctl(port, TIOCMSET, &status) == -1) {
		tcsetattr(port, TCSANOW, &old_port_settings);
		flock(port, LOCK_UN);
		perror("unable to set portstatus");
		return INVALID_COMPORT;
	}

	return port;
}


int serial_poll(comport_t port, unsigned char *buf, int size)
{
	int n = read(port, buf, size);
	if(n < 0) {
		if(errno == EAGAIN) return SERIAL_ERROR;
	}

	return n;
}

void serial_flush(comport_t port)
{
	/*
		options:
		TCIFLUSH — flush input queue (discard data received but not read).
		TCOFLUSH — flush output queue (discard data written but not transmitted).
		TCIOFLUSH — flush both input and output queues.
	*/
	if (tcflush(port, TCIOFLUSH) == -1) {
		fprintf(stderr, "failed to flush serial buffer\n");
	}
}

int serial_send_byte(comport_t port, unsigned char byte)
{
	int n = write(port, &byte, 1);
	return (n > -1 ? SERIAL_OK : SERIAL_ERROR);
}

int serial_send_buf(comport_t port, unsigned char *buf, int size)
{
	int n = write(port, buf, size);
	return (n > -1 ? SERIAL_OK : SERIAL_ERROR);
}

void serial_close(comport_t port)
{
	int status;

	if (ioctl(port, TIOCMGET, &status) == -1) {
		perror("unable to get portstatus");
	}

	/* turn off DTR */
	status &= ~TIOCM_DTR;
	/* turn off RTS */
	status &= ~TIOCM_RTS;

	if (ioctl(port, TIOCMSET, &status) == -1) {
		perror("unable to set portstatus");
	}

	// can't restore previous port settings without holding array of termios structs
	// perhaps pass this to user to hold on to.
	//tcsetattr(port_num, TCSANOW, &old_port_settings);
	
	close(port);

	/* free the port so that others can use it. */
	flock(port, LOCK_UN);
}

void serial_break(comport_t port, int delay_ms)
{
	if(ioctl(port, TIOCSBRK, NULL) == -1) {
		perror("unable to turn break on");
	}

	sleep(1);

	if(ioctl(port, TIOCCBRK, NULL) == -1) {
		perror("unable to turn break off");
	}
}

#endif

int serial_cputs(comport_t port, const char *string)
{
	if (string == NULL) return SERIAL_ERROR;
	while (*string != 0) if (serial_send_byte(port, *(string++)) == SERIAL_ERROR) return SERIAL_ERROR;
	return SERIAL_OK;
}
