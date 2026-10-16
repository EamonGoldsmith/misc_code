#ifndef STRING_BUILDER_H_
#define STRING_BUILDER_H_

#define INITIAL_STACK_SIZE 4096

#include <stddef.h>

typedef struct 
{
	// can be passed into slice functions
	char *str;
	size_t len;

	// track initail stack and heap allocation
	size_t capacity;
	char stack[INITIAL_STACK_SIZE];

	// sticky error, so its safe to call append many times
	// without checking for oom
	int error;
} sb_t;

void sb_init(sb_t *sb);
void sb_appendf(sb_t *sb, const char *fmt, ...);
void sb_terminate(sb_t *sb);
size_t sb_mark(sb_t *sb);
void sb_rewind(sb_t *sb, size_t mark);
void sb_clear(sb_t *sb);
void sb_free(sb_t *sb);

#endif // STRING_BUILDER_H_
