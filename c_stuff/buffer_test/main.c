#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define RING_SIZE 64U

typedef struct {
	int write;
	int read;
	int size;
	char buffer[RING_SIZE];
} ring_t;

void wr(ring_t *r, char *str, int len)
{
	r->size++;

	// if the message won't overrun the buffer, its easy
	if (len + r->write <= RING_SIZE) {
		memcpy(r->buffer + r->write, str, len);
		r->write += len;
		return;
	}

	// otherwise, do the first part
	const int diff = RING_SIZE - r->write;
	memcpy(r->buffer + r->write, str, diff);
	len -= diff;
	r->write = len;

	// then do the next part
	memcpy(r->buffer, str + diff, len);

	// place write marker on the end of the last string
	r->write++;
	if (r->write > RING_SIZE)
		r->write = 0;
}

int rd(ring_t *r, char **str, int *len)
{
	// check buffer not empty
	if (r->size <= 0) {
		printf("read failed, no elements in buffer\n");
		return 0;
	}

	r->size--;

	// return element
	*str = r->buffer + r->read;
	*len = strlen(*str);

	// move read marker to next position
	char c;
	while ((c = r->buffer[r->read]) != '\0') {
		if (r->read == 0) { r->read = RING_SIZE; };
		r->read--;
	}

	// place read marker on start of next string
	r->read--;
	if (r->read < 0)
		r->read = RING_SIZE;

	return 1;
}

void print_ring(ring_t *r)
{
	//printf("write marker: %i\n", r->write);
	//printf("read marker: %i\n", r->read);
	printf("contents: ");

	for (int i = 0; i < RING_SIZE; i++) {
		if (r->buffer[i] == 0)
			printf("%s", ".");
		else
			printf("%c", r->buffer[i]);
	}
	printf("\n");
}

int main()
{
	ring_t my_ring;

	wr(&my_ring, "Hello", 6);
	print_ring(&my_ring);

	wr(&my_ring, "This is a very long string to pad out some space", 49);
	print_ring(&my_ring);

	// write a bunch
	for (int i = 0; i < 3; i++) {
		wr(&my_ring, "TESTING", 8);
		print_ring(&my_ring);
	}

	// read a bunch
	for (int i = 0; i < 200; i++) {
		char *string; int len;
		if (rd(&my_ring, &string, &len)) {
			printf("%i, %s\n", len, string);
		}
	}

	return 0;
}

