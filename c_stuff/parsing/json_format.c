#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	void* ptr;
	void* offset;
	unsigned int capacity;
} Arena;

Arena* arena_create(unsigned int bytes) 
{
	Arena* arena = (Arena*)malloc(sizeof(Arena) + bytes);
	arena->ptr = arena + 1;
	if (arena->ptr) {
		arena->capacity = bytes;
		arena->offset = 0;
		return arena;
	}
	arena->ptr = NULL;
	arena->capacity = 0;
	arena->offset = 0;
	return NULL;
}

void* arena_alloc(Arena* arena, unsigned int bytes)
{
	if ((arena->offset - arena->ptr) + bytes > arena->capacity) {
		return NULL;
	}

	void* tmp = (void*)((size_t)arena->ptr + (size_t)arena->offset);
	arena->offset += bytes;
	return tmp;
}

typedef struct
{
	char *str;
	int len;
} String_View;

String_View convert_num_arr(Arena* arena, char* label, int* arr, int len)
{
	char *ptr, *str;
	ptr = str = arena_alloc(arena, strlen(label) + 4);
	if (!ptr) {
		return (String_View){ NULL, 0 };
	}
	ptr = memcpy(ptr, "{\"", 2) + 2;
	ptr = memcpy(ptr, label, strlen(label)) + strlen(label);
	ptr = memcpy(ptr, "\":", 2) + 2;

	ptr = arena_alloc(arena, (3 * len) + 3);
	if (!ptr) {
		return (String_View){ NULL, 0 };
	}
	ptr[0] = '['; ptr++;
	for (int i = 0; i < len; i++) {
		int c = (arr[i] & 0xf0) >> 4;
		ptr[0] = c > 9 ? c + 'a' - 9 : c + '0';
		c = (arr[i] & 0x0f);
		ptr[1] = c > 9 ? c + 'a' - 9 : c + '0';
		if (i + 1 < len) {
			ptr[2] = ',';
			ptr += 3;
		} else {
			ptr += 2;
		}
	}
	ptr[0] = ']';
	ptr[1] = '}';

	return (String_View){ str, (int)(ptr - str) };
}

String_View convert_text_arr(Arena* arena, char *label, String_View* arr, int len)
{
	char *ptr, *str;
	ptr = str = arena_alloc(arena, strlen(label) + 5);
	if (!ptr) {
		return (String_View){ NULL, 0 };
	}
	ptr = memcpy(ptr, "{\"", 2) + 2;
	ptr = memcpy(ptr, label, strlen(label)) + strlen(label);
	ptr = memcpy(ptr, "\":[", 3) + 3;

	for (int i = 0; i < len; i++) {
		ptr = arena_alloc(arena, arr[i].len + 1);
		if (!ptr) {
			return (String_View){ NULL, 0 };
		}
		memcpy(ptr, arr[i].str, arr[i].len);
		ptr += arr[i].len;
		if (i + 1 < len) {
			ptr[0] = ',';
		} else {
			ptr[0] = ']';
			ptr[1] = '}';
		}
	}
	return (String_View){ str, (int)(ptr - str) };
}

int main()
{
	int example_numbers[] = {1, 2, 3, 4, 5, 6, 7, 8};

	String_View example_filenames[] = {
		{"abc.csv", 7},
		{"othername.csv", 13},
		{"cab.csv", 7},
		{"bac.csv", 7},
		{"aaa.csv", 7},
		{"bbb.csv", 7},
		{"ccc.csv", 7}
	};

	Arena *arena = arena_create(2048);
	if (!arena) {
		return 1;
	}
	String_View json_numbers = convert_num_arr(arena, "stuff", example_numbers, 8);
	if (!json_numbers.str) {
		printf("failed to format\n");
		return 1;
	}
	printf("%*s\n", json_numbers.len, json_numbers.str);

	free(arena);

	arena = arena_create(2048);
	if (!arena) {
		return 1;
	}
	String_View json_files = convert_text_arr(arena, "files", example_filenames, 7);
	if (!json_files.str) {
		printf("failed to format\n");
		return 1;
	}
	printf("%*s\n", json_files.len, json_files.str);
	arena_free(arena);
	return 0;
}
