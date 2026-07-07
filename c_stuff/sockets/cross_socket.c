
#include "cross_socket.h"

#ifdef _WIN32

int cross_init()
{
	WSADATA wsa_data;
    return WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0;
}

socket_t cross_connect(char *address, unsigned int port)
{
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory( &hints, sizeof(hints) );

	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// resolve the server address and port
	if (getaddrinfo(address, port, &hints, &result) != 0) {
		cross_cleanup()
		return INVALID_SOCKET;
	}
	
	socket_t sock = INVALID_SOCKET;

	sock = socket(
		result->ai_family,
		result->ai_socktype,
		result->ai_protocol
	);

	if (sock == INVALID_SOCKET) {
		freeaddrinfo(result);
		cross_cleanup();
		return INVALID_SOCKET;
	}

	if (connect(sock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		closesocket(sock);
		sock = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (sock == INVALID_SOCKET) {
		cross_cleanup();
		return INVALID_SOCKET;
	}

	return sock;
}

int cross_send(socket_t sock, char *buffer, size_t len)
{
	return send(sock, buffer, len, 0);
}

int cross_recv(socket_t sock, char *buffer, size_t *len)
{
	return recv(sock, buffer, len, 0);
}

void cross_close(socket_t sock)
{
	closesocket(sock);
}

void cross_cleanup()
{
	WSACleanup();
}

#else

void cross_resolve(socket_t sock, char *hostname, char *address);

int cross_init()
{
	// not required on posix
	return 1;
}

socket_t cross_connect(char *address, unsigned int port)
{
	socket_t sock;

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(port),
	};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		return SOCKET_ERROR;
	}

	int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) {
		return SOCKET_ERROR;
    }
    
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        return SOCKET_ERROR;
    }

	int res;

	if (inet_pton(AF_INET, address, &addr.sin_addr))
	{
		res = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	}
	else
	{
		char resolved[IPV6_STRING_LENGTH];
		if (cross_resolve(sock, address, resolved) == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}

		inet_pton(AF_INET, resolved, &addr.sin_addr)
		if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
	}

	// wait for socket to finish connecting before trying to check error
	sleep(1);

	if (res < 0 && errno != EINPROGRESS) {
		return SOCKET_ERROR;
	}

	return sock;
}

int cross_send(socket_t sock, char *buffer, size_t len)
{
	return send(sock, buffer, len, 0);
}

int cross_recv(socket_t sock, char *buffer, size_t *len)
{
	return recv(sock, buffer, len, 0);
}

void cross_close(socket_t sock)
{
	close(sock);
}

int cross_resolve(socket_t sock, char *hostname, char *address)
{
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, NULL, &hints, &result) != 0) {
        return SOCKET_ERROR;
    }

	// iterator ?!
    for (p = result; p != NULL; p = p->ai_next)
	{
        int *res = inet_ntop(
			p->ai_family,
			&(p->ai_addr->sin_addr),
			address,
			IPV6_STRING_LENGTH
		);

		if (res != NULL) {
            freeaddrinfo(res);
			return 1;
        }
    }

    freeaddrinfo(result);
    return SOCKET_ERROR;
}

void cross_cleanup()
{
	// not needed on posix
	return;
}

#endif
