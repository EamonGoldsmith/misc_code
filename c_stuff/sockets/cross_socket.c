
#include "cross_socket.h"

#ifdef _WIN32

#define MAX_PORT_LEN 20
#include <stdio.h>

int cross_init()
{
	WSADATA wsa_data;
    return WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0;
}

void cross_cleanup()
{
	WSACleanup();
}

socket_t cross_server(unsigned int port)
{
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int res;

	// convert port number to string
	char port_str[MAX_PORT_LEN];
	snprintf(port_str, MAX_PORT_LEN, "%u", port);

	// resolve the local address and port to be used by the server
	res = getaddrinfo(NULL, port_str, &hints, &result);
	if (res != 0) {
		fprintf(stderr, "getaddrinfo failed: %d\n", res);
		cross_cleanup();
		return INVALID_SOCKET;
	}

	// create socket for server to listen on
	socket_t server = socket(
		result->ai_family,
		result->ai_socktype,
		result->ai_protocol
	);

	if (server == INVALID_SOCKET) {
		fprintf(stderr, "failed to create socket: %s\n", GET_SOCKET_ERR());
		freeaddrinfo(result);
		cross_cleanup();
		return INVALID_SOCKET;
	}

	// bind socket
	res = bind(server, result->ai_addr, (int)result->ai_addrlen);
	if (res == SOCKET_ERROR) {
		fprintf(stderr, "failed to bind: %s\n", GET_SOCKET_ERR());
		freeaddrinfo(result);
		closesocket(server);
		cross_cleanup();
		return INVALID_SOCKET;
	}

	freeaddrinfo(result);

	// listen on socket
	if (listen(server, MAX_CLIENTS) == SOCKET_ERROR) {
		fprintf(stderr, "failed to listen: %s\n", GET_SOCKET_ERR());
		closesocket(server);
		cross_cleanup();
		return INVALID_SOCKET;
	}

	return server;
}

socket_t cross_accept(socket_t server)
{
    socket_t client = accept(server, NULL, NULL);

    if (client == INVALID_SOCKET) {
        perror("accept");
        return INVALID_SOCKET;
    }

	return client;
}

socket_t cross_connect(const char *address, unsigned int port)
{
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory( &hints, sizeof(hints) );

	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// convert port number to string
	char port_str[MAX_PORT_LEN];
	snprintf(port_str, MAX_PORT_LEN, "%u", port);

	// resolve the server address and port
	if (getaddrinfo(address, port_str, &hints, &result) != 0) {
		fprintf(stderr, "failed to resolve host\n");
		cross_cleanup();
		return INVALID_SOCKET;
	}
	
	socket_t sock = INVALID_SOCKET;

	sock = socket(
		result->ai_family,
		result->ai_socktype,
		result->ai_protocol
	);

	if (sock == INVALID_SOCKET) {
		fprintf(stderr, "failed to create socket\n");
		freeaddrinfo(result);
		cross_cleanup();
		return INVALID_SOCKET;
	}

	if (connect(sock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		fprintf(stderr, "failed to connect socket\n");
		closesocket(sock);
		sock = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (sock == INVALID_SOCKET) {
		fprintf(stderr, "socket failed to connect\n");
		cross_cleanup();
		return INVALID_SOCKET;
	}

	return sock;
}


int cross_send(socket_t sock, const char *buffer, size_t len)
{
	return send(sock, buffer, len, 0);
}

int cross_recv(socket_t sock, const char *buffer, size_t len)
{
	return recv(sock, buffer, len, 0);
}

void cross_close(socket_t sock)
{
	closesocket(sock);
}

int cross_resolve(socket_t sock, const char *hostname, char *address)
{
	// don't need this, windows resolves on connect
	return SOCKET_ERROR;
}

#else

// not required on posix
int cross_init() { return 1; }
void cross_cleanup() { return; }

socket_t cross_server(unsigned int port)
{
	// create socket
    socket_t server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server < 0) {
        perror("socket failed");
        return INVALID_SOCKET;
    }

	// set socket to reuse address
    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(port),
    };

	// bind socket
    if (bind(server, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return INVALID_SOCKET;
    }

	// listen
    if (listen(server, MAX_CLIENTS) < 0) {
        perror("listen");
        return INVALID_SOCKET;
    }

	return server;
}

socket_t cross_accept(socket_t server)
{
	struct sockaddr_in address;
	unsigned int addr_len = sizeof(address);

	// accept connection
    socket_t client = accept(server, (struct sockaddr*)&address, &addr_len);

    if (!IS_VALID_SOCKET(client)) {
        perror("accept");
        return INVALID_SOCKET;
    }

    return client;
}

socket_t cross_connect(const char *address, unsigned int port)
{
	socket_t sock;

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(port),
	};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return SOCKET_ERROR;
	}

	int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) {
		perror("get flags");
		return SOCKET_ERROR;
    }
    
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
		perror("set flags");
        return SOCKET_ERROR;
    }

	int res;

	if (inet_pton(AF_INET, address, &addr.sin_addr)) {
		res = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	}

	sleep(1);

	if (res < 0 && errno != EINPROGRESS) {
		perror("connect");
		return SOCKET_ERROR;
	}

	return sock;
}

int cross_send(socket_t sock, const char *buffer, size_t len)
{
	return send(sock, buffer, len, 0);
}

int cross_recv(socket_t sock, const char *buffer, size_t len)
{
	return recv(sock, buffer, len, 0);
}

void cross_close(socket_t sock) { close(sock); }

#endif
