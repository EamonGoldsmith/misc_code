#include "serial.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>

#if defined(__linux__)
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <termios.h>
#else
#include <windows.h>
#endif

// man 3 termios
// man 4 tty_ioctl

size_t serial_open(const char *devname, int baudrate, const char *mode, int flowctrl)
{
	size_t port_num;

#if defined(__linux__)
	int err,
		baudr,
		status;

	int cbits=CS8,
		cpar=0,
		ipar=IGNPAR,
		bstop=0;

	if (strlen(mode) != 3) {
		ERR_LOG("invalid mode");
		return 0;
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
		ERR_LOG("invalid number of data-bits");
		return 0;
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
		ERR_LOG("invalid parity");
		return 0;
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
		ERR_LOG("invalid number of stop bits");
		return 0;
	}

	port_num = open(devname, O_RDWR | O_NOCTTY | O_NDELAY);
	
	if (port_num == -1) {
		perror("unable to open comport");
		return 0;
	}

	// lock access so that another process can't also use the port
	if (flock(port_num, LOCK_EX | LOCK_NB) != 0) {
		close(port_num);
		perror("another process has locked the comport");
		return 0;
	}

	struct termios old_port_settings;
	struct termios new_port_settings;

	// backup port settings
	err = tcgetattr(port_num, &old_port_settings);
	if (err==-1) {
		close(port_num);
		flock(port_num, LOCK_UN); // free the port so that others can use it.
		perror("unable to read portsettings");
		return 0;
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
	err = tcsetattr(port_num, TCSANOW, &new_port_settings);
	if (err == -1) {
		tcsetattr(port_num, TCSANOW, &old_port_settings);
		close(port_num);
		flock(port_num, LOCK_UN);
		perror("unable to adjust portsettings");
		return 0;
	}

	// update with custom non-standard rate
	struct serial_struct serial;
	if (ioctl(port_num, TIOCGSERIAL, &serial) == -1) {
		tcsetattr(port_num, TCSANOW, &old_port_settings);
		flock(port_num, LOCK_UN);
		perror("unable to get portstatus");
		return 0;
	}
	
	serial.flags &= ~ASYNC_SPD_MASK;
	serial.flags |= ASYNC_SPD_CUST;
	serial.custom_divisor = serial.baud_base / baudrate;

	if (ioctl(port_num, TIOCSSERIAL, &serial) == -1) {
		tcsetattr(port_num, TCSANOW, &old_port_settings);
		flock(port_num, LOCK_UN);
		perror("unable to get portstatus");
		return 0;
	}

	// get status
	if (ioctl(port_num, TIOCMGET, &status) == -1) {
		tcsetattr(port_num, TCSANOW, &old_port_settings);
		flock(port_num, LOCK_UN);
		perror("unable to get portstatus");
		return 0;
	}

	// turn on DTR
	status |= TIOCM_DTR;
	// turn on RTS
	status |= TIOCM_RTS;

	// set modem bits
	if (ioctl(port_num, TIOCMSET, &status) == -1) {
		tcsetattr(port_num, TCSANOW, &old_port_settings);
		flock(port_num, LOCK_UN);
		perror("unable to set portstatus");
		return 0;
	}

#else
	char mode_str[512];
	sprintf(mode_str, "baud=%i data=%i parity=%c stop=%i xon=off to=off odsr=off dtr=on rts=%s",
		baudrate, mode[0] - '0', mode[1], mode[2] - '0',
		flowctrl ? "on" : "off");

	DEBUG_LOG("mode_str: %s", mode_str);

	// devname must have form "\\\\.\\COMX"
	port_num = (size_t)CreateFileA(devname, 
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if ((void*)port_num == INVALID_HANDLE_VALUE) {
		ERR_LOG("unable to open port: %s", devname);
		return 0;
	}

	DCB port_settings;
	memset(&port_settings, 0, sizeof(port_settings));
	port_settings.DCBlength = sizeof(port_settings);

	if (!BuildCommDCBA(mode_str, &port_settings)) {
		ERR_LOG("failed to set port DCB settings");
		CloseHandle((void*)port_num);
		return 0;
	}

	if (flowctrl) {
		port_settings.fOutxCtsFlow = TRUE;
		port_settings.fRtsControl = RTS_CONTROL_HANDSHAKE;
	}
 
	if (!SetCommState((void*)port_num, &port_settings)) {
		ERR_LOG("failed to set port settings");
		CloseHandle((void*)port_num);
		return 0;
	}

	COMMTIMEOUTS Cptimeouts;
	Cptimeouts.ReadIntervalTimeout = MAXDWORD;
	Cptimeouts.ReadTotalTimeoutMultiplier = 0;
	Cptimeouts.ReadTotalTimeoutConstant = 0;
	Cptimeouts.WriteTotalTimeoutMultiplier = 0;
	Cptimeouts.WriteTotalTimeoutConstant = 0;

	if (!SetCommTimeouts((void*)port_num, &Cptimeouts)) {
		ERR_LOG("failed to set port timeout");
		CloseHandle((void*)port_num);
		return 0;
	}
#endif

	return port_num;
}


int serial_poll(size_t port_num, unsigned char *buf, int size)
{
	int n;

#if defined(__linux__)
	n = read(port_num, buf, size);

	if(n < 0) {
		if(errno == EAGAIN)  return 0;
	}
#else
	if (!ReadFile((void*)port_num, buf, size, (LPDWORD)((void*)&n), NULL)) {
		return 0;
	}
#endif

	return n;
}

void serial_flush(size_t port_num)
{
#if defined(__linux__)
	/*
		options:
		TCIFLUSH — flush input queue (discard data received but not read).
		TCOFLUSH — flush output queue (discard data written but not transmitted).
		TCIOFLUSH — flush both input and output queues.
	*/

	if (tcflush(port_num, TCIOFLUSH) == -1) {
		ERR_LOG("failed to flush serial buffer");
	}
#else
	PurgeComm((void*)port_num, PURGE_RXCLEAR | PURGE_RXABORT);
	PurgeComm((void*)port_num, PURGE_TXCLEAR | PURGE_TXABORT);
#endif

	return;
}

int serial_send_byte(size_t port_num, unsigned char byte)
{
	int n;

#if defined(__linux__)
	n = write(port_num, &byte, 1);

#else
	if (!WriteFile((void*)port_num, &byte, 1, (LPDWORD)((void*)&n), NULL)) {
		return 0;
	}

#endif

	return (n > -1);
}

int serial_send_buf(size_t port_num, unsigned char *buf, int size)
{
	int n;

#if defined(__linux__)
	n = write(port_num, buf, size);

#else
	if (!WriteFile((void*)port_num, buf, size, (LPDWORD)((void*)&n), NULL)) {
		return 0;
	}

#endif

	return (n > -1);
}

void serial_close(size_t port_num)
{
#if defined(__linux__)
	int status;

	if (ioctl(port_num, TIOCMGET, &status) == -1) {
		perror("unable to get portstatus");
	}

	/* turn off DTR */
	status &= ~TIOCM_DTR;
	/* turn off RTS */
	status &= ~TIOCM_RTS;

	if (ioctl(port_num, TIOCMSET, &status) == -1) {
		perror("unable to set portstatus");
	}

	// can't restore previous port settings without holding array of termios structs
	// perhaps pass this to user to hold on to.
	//tcsetattr(port_num, TCSANOW, &old_port_settings);
	
	close(port_num);

	/* free the port so that others can use it. */
	flock(port_num, LOCK_UN);
#else
	CloseHandle((void*)port_num);

#endif
}

void serial_break(size_t port_num, int delay_ms)
{
#if defined(__linux__)
	if(ioctl(port_num, TIOCSBRK, NULL) == -1) {
		perror("unable to turn break on");
	}

	sleep(1);

	if(ioctl(port_num, TIOCCBRK, NULL) == -1) {
		perror("unable to turn break off");
	}

#else
	if (!SetCommBreak((void*)port_num)) {
		ERR_LOG("failed to set break");
	}

	sleep(1);

	if (!ClearCommBreak((void*)port_num)) {
		ERR_LOG("failed to clear break");
	}

#endif
}

// sends a string to serial port
void serial_cputs(size_t port_num, const char *text)
{
	while (*text != 0) serial_send_byte(port_num, *(text++));
}
