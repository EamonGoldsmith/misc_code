/*
    ring_t holds array of elements.
    element_t has length and pointer to buffer

    user will need to create an array of element_t and a data buffer to use ringbuffer.


    requirements?
    -   stack only
    -   no specific type
    -   the caller doesn't need to know the size of the ring or elements
    -   read and write operations take a buffer and a length
    -   can get a reference to underlying array
*/

typedef struct
{
    unsigned int length;
    void *buffer;
} element_t;

typedef struct
{
    // markers
    unsigned int read;
    unsigned int write;

    unsigned int length;

    unsigned int element_size;
    unsigned int ring_size;

    element_t *data;
} ring_t;


void ring_init(ring_t *ring,
    unsigned int element_size, unsigned int ring_size,
    element_t *data, void *buffer);

// true if the ring is empty, false if there are elements
int ring_is_empty(ring_t *ring);

void ring_write(ring_t *ring, void *buffer, unsigned int length);

// use NULL as buffer and length term if you don't care about contents and want to move to next
void ring_read(ring_t *ring, void *buffer, unsigned int *length);

// get the next element for reading without changing markers
void ring_peek(ring_t *ring, void *buffer, unsigned int *length);

// get the address of the write marker in the buffer
void ring_wpeek(ring_t *ring, void *buffer);