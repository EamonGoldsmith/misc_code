#ifndef MAP_H_
#define MAP_H_

typedef struct map_t
{
	void *index;
	void *value;
	struct map_t *next;
} map_t;

int create_map(map_t *m);
int map_get(map_t *m, void *index, void *value);
int map_add(map_t *m, void *index, void *value);
void destroy_map(map_t *m);
void print_map(map_t *m);

#endif // MAP_H_

