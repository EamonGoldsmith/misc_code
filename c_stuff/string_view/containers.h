#ifndef CONTAINERS_H_
#define CONTAINERS_H_

/*
goal:
	single header library to provide simple hidden header
	sized container for C. written in pure C. depends on malloc.
	provides; push_back, destroy, length, foreach(...), rforeach(), itbegin, itend
*/

#include <stdlib.h>

typedef struct
{
	size_t size;
	size_t capacity;
} Header;

#define ARR_INITIAL_CAPACITY 2

#define push_back(arr, el) \
do { \
	if (arr == NULL) { \
		Header* header = malloc(sizeof(*arr)*ARR_INITIAL_CAPACITY + sizeof(Header)); \
		header->size = 0; \
		header->capacity = ARR_INITIAL_CAPACITY; \
		(arr) = (void*)(header + 1); \
	} \
	Header* header = (Header*)(arr) - 1; \
	if (header->size >= header->capacity) { \
		header->capacity *= 2; \
		header = realloc(header, sizeof(*arr)*header->capacity + sizeof(Header)); \
		(arr) = (void*)(header + 1); \
	} \
	(arr)[header->size++] = (el); \
} while (0); \

#define destroy(arr) \
	free((Header*)(arr) - 1); arr = NULL

#define length(arr) \
	((Header*)(arr) - 1)->size

#define itbegin(arr) \
	arr

#define itend(arr) \
	arr + length(arr)

#define foreach(type, it, arr) \
	for (type it = itbegin(arr); it != itend(arr); it++)

#define rforeach(type, it, arr) \
	for (type it = itend(arr) - 1; it != itbegin(arr) - 1; it--)

#define enumerate(en, type, it, arr) \
	size_t en = 0; \
	for (type it = itbegin(arr); it != itend(arr); it++, en++)

#define reverse_enumerate(en, type, it, arr) \
	size_t en = length(arr); \
	for (type it = itend(arr) - 1; it != itbegin(arr) - 1; it--, en--)

#endif // CONTAINERS_H_
