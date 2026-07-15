
#include <stdio.h>
#include <stdlib.h>

#define get_header(ptr, header) \
	

#define make_shared(ptr) \
	__attribute__((cleanup (drop(ptr))))
	

typedef struct
{
	size_t references;
	void (dealloc*)
} smart_ptr;

void some_func(void *ptr)
{
	printf("creating shared pointer: %p", ptr);
	void *smart = make_shared(ptr);
}

void demo_smart_ptr(void *ptr)
{
	some_func(ptr);
	printf("shared pointer dealloced: %p", ptr);
}

int main()
{
	// open a file to test with
	FILE *fd = NULL;
	fd = open("test.txt", "r");
	if (fd == NULL) {
		perror("fopen failed");
		exit(1);
	}

	struct stat filestat;

	if (fstat(fd, &filestat) != 0) {
		perror("stat failed");
		exit(1);
	}

	void *data = mmap(NULL, filestat.st_size, PROT_READ, MAP_SHARED, fp, 0);
	if (data == MAP_FAILED) {
		perror("mmap failed");
		exit(2);
	}

	demo_smart_ptr(data);


	return 0;
}
