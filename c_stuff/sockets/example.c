#include "cross_socket.h"

#include <stdio.h>
#include <time.h>

#define PORT 8087

int main()
{
	if (!cross_init()) {
		printf("failed: %s\n", GET_SOCKET_ERR());
		return 1;
	}

	socket_t server = cross_server(PORT);

	while (1)
	{
		printf("waiting...\n");

		socket_t client = cross_accept(server);

		if (!IS_VALID_SOCKET(client)) 
		{
			printf("failed to connect\n");
			cross_cleanup();
			break;
		}

		printf("connected\n");

		char *hello = "hello, world!\n";

		if (cross_send(client, hello, 14) < 0)
		{
			cross_close(client);
			cross_close(server);
			cross_cleanup();
			return 1;
		}

		printf("sent\n");
		cross_close(client);
	}

	cross_close(server);
	cross_cleanup();
	return 0;
}
