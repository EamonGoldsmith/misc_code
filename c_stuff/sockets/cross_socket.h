#ifndef CROSS_SOCKET_H_
#define CROSS_SOCKET_H_

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    
    typedef SOCKET socket_t;
    #define IS_VALID_SOCKET(s) ((s) != INVALID_SOCKET)
    #define GET_SOCKET_ERR() WSAGetLastError()

#else
	#include <arpa/inet.h>
	#include <errno.h>
	#include <fcntl.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <sys/socket.h>
	#include <sys/time.h>
	#include <sys/types.h>
	#include <unistd.h>
			
    typedef int socket_t;
    #define IS_VALID_SOCKET(s) ((s) >= 0)
    #define GET_SOCKET_ERR() (errno)
    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR (-1)
#endif

#define IPV6_STRING_LENGTH 40

int cross_init();
socket_t cross_connect(char *address, unsigned int port);
int cross_send(socket_t *sock, char *buffer, size_t len);
int cross_recv(socket_t *sock, char *buffer, size_t *len);
void cross_close(socket_t *sock);
int cross_resolve(socket_t *sock, char *hostname, char *address);
void cross_cleanup();

#endif // CROSS_SOCKET_H_
