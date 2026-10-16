#include "string_builder.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void sb_init(sb_t *sb)
{
	sb->str = sb->stack;
	sb->len = 0;
	sb->capacity = INITIAL_STACK_SIZE;
	sb->error = 0;
}

void sb_appendf(sb_t *sb, const char *fmt, ...)
{
	va_list ap;

	// get length of formatted string
	va_start(ap, fmt);
	int length = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	if (length < 0) {
		sb->error++;
		return;
	}
	
	// check if we need to allocate new space
	if (length + sb->len > sb->capacity)
	{
		size_t new_capacity = length + (2 * sb->capacity);

		if (sb->str == sb->stack) {
			sb->str = malloc(new_capacity);
		} else {
			sb->str = realloc(sb->str, new_capacity);
		}

		if (sb->str == NULL) {
			sb->str = sb->stack;
			sb->error++;
			return;
		}

		sb->capacity = new_capacity;
	}

	int term = (sb->str[sb->len - 1] == '\0' && sb->len > 0);

	// copy formatted string into str
	va_start(ap, fmt);
	length = vsnprintf(sb->str + sb->len - term, length + 1, fmt, ap);
	va_end(ap);

	sb->len += length;
}

void sb_terminate(sb_t *sb) { return; }
size_t sb_mark(sb_t *sb) { return sb->len; }
void sb_rewind(sb_t *sb, size_t mark) { sb->len = mark; }

void sb_clear(sb_t *sb)
{
	for (int i = 0; i < sb->len; i++) {
		sb->str[i] = 0;
	}

	sb->len = 0;
	sb->error = 0;

	// clear str
	for (size_t i = 0; i < sb->capacity; i++) {
		sb->stack[i] = 0;
	}
}

void sb_free(sb_t *sb)
{
	// we only need to call free if heap space was allocated
	if (sb->str != sb->stack) {
		free(sb->str);
		sb->str = sb->stack;
	}

	sb->len = 0;
	sb->capacity = INITIAL_STACK_SIZE;
	sb->error = 0;

	// clear stack str
	for (size_t i = 0; i < INITIAL_STACK_SIZE; i++) {
		sb->stack[i] = 0;
	}
}

