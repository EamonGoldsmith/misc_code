#include <stdio.h>

#include "ring_buffer.h"

#define ELEMENT_SIZE (10U)
#define RING_SIZE (10U)

ring_t myRingBuffer;

// create the data buffer, can be whatever type
unsigned char data_buffer[ELEMENT_SIZE * RING_SIZE];

// create the element buffer, must be element_t
element_t element_buffer[RING_SIZE];

int main()
{
    // test strings
    char *stringA = "Hello, world!"; int lenA = 10;
    char *stringB = "Testing"; int lenB = 7;
    char *stringC = "goodbye!"; int lenC = 8;
    char *stringD = "something"; int lenD = 9;

    ring_init(&myRingBuffer,
        ELEMENT_SIZE, RING_SIZE,
        element_buffer, data_buffer);

    ring_write(&myRingBuffer, stringA, lenA);
    ring_write(&myRingBuffer, stringB, lenB);
    ring_write(&myRingBuffer, stringC, lenC);
    ring_write(&myRingBuffer, stringD, lenD);
    
    char *str; int len;

    ring_read(&myRingBuffer, &str, &len);
    printf("read: (%.*s)\n", len, str);
    ring_read(&myRingBuffer, &str, &len);
    printf("read: (%.*s)\n", len, str);
    ring_read(&myRingBuffer, &str, &len);
    printf("read: (%.*s)\n", len, str);
    ring_read(&myRingBuffer, &str, &len);
    printf("read: (%.*s)\n", len, str);

    if (!ring_is_empty(&myRingBuffer)) {
        ring_read(&myRingBuffer, &str, &len);
        printf("read: (%.*s)\n", len, str);
    }
    
	return 0;
}