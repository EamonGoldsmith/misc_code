#include <stdio.h>
#include <string.h>

#include "ring_buffer.h"

void ring_init(ring_t *ring,
    unsigned int element_size, unsigned int ring_size,
    element_t *data, void *buffer)
{
    if (ring == NULL)
        return;

    ring->read = 0;
    ring->write = 0;
    ring->length = 0;

    ring->element_size = element_size;
    ring->ring_size = ring_size;

    // clear both buffers
    memset(data, 0, element_size * ring->ring_size);
    memset(buffer, 0, ring->ring_size);

    // assign element pointers to buffer locations
    for (size_t i = 0; i < ring->ring_size; i++) {
        data[i].buffer = buffer + (i * element_size);
    }

    ring->data = data;
}

int ring_is_empty(ring_t *ring)
{
    if (ring == NULL)
        return 0;

    return ring->length == 0;
}

void ring_write(ring_t *ring, void *buffer, unsigned int length)
{
    if (ring == NULL)
            return;

    // check we can fit another element
    if (ring->length < ring->ring_size)
        ring->length++;

    element_t *next = &ring->data[ring->write];

    // copy to location, clip size
    unsigned int len = ring->element_size > length ? ring->element_size : length;
    memcpy(next->buffer, buffer, len);
    next->length = len;
    
    ring->write++;
    ring->write %= ring->ring_size;
}

void ring_read(ring_t *ring, void *buffer, unsigned int *length)
{
    if (ring == NULL)
        return;

    if (ring_is_empty(ring))
        return;

    ring_peek(ring, buffer, length);

    ring->read++;
    ring->read %= ring->ring_size;

    ring->length--;
}

void ring_peek(ring_t *ring, void *buffer, unsigned int *length)
{
    element_t current = ring->data[ring->read];
    *(void**)buffer = current.buffer;
    *length = current.length;
}

void ring_wpeek(ring_t *ring, void *buffer)
{
    if (ring == NULL)
        return;

    element_t current = ring->data[ring->write];
    buffer = current.buffer;
}


