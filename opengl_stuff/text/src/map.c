#include "map.h"

#include <stdio.h>
#include <stdlib.h>

int create_map(map_t *m)
{
	m->index = NULL;
	m->value = NULL;
	m->next = NULL;

	return 1;
}

int map_get(map_t *m, void *index, void *value)
{
	// validate input
	if (m == NULL) {
		return 0;
	}

	// walk the list
	map_t *n = m->next;
	while (n != NULL) {
		if (n->index != NULL && n->index == index) {
			*(void**)value = *(void**)n->value;
			return 1;
		}

		n = n->next;
	}

	return 0;
}

int map_add(map_t *m, void *index, void *value)
{
	// validate input
	if (m == NULL) {
		return 0;
	}

	// walk to end of list
	map_t *n = m;
	while (n->next != NULL) {
		n = n->next;
	}

	// add new map node
	map_t *new = (map_t*)malloc(sizeof(map_t));
	new->index = index;
	new->value = value;
	n->next = new;

	return 1;
}

void destroy_map(map_t *m)
{
	// don't free the first node in the map
	if (m->next == NULL) {
		return;
	}

	// walk to the end, deleting each node
	map_t *n = m->next;

	while (n->next != NULL) {
		map_t *tmp = n->next;
		n->next = NULL;
		free(n);
		n = tmp;
	}

	m->next = NULL;
}

void print_map(map_t *m)
{
	while (m != NULL) {
		printf("index: %p, value: %p\n", m->index, m->value);
		m = m->next;
	}
}
