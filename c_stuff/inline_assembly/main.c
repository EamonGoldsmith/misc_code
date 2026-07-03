#include <stdio.h>

int add(int a, int b) {
	int result;

	asm("addl %%ebx, %%eax;"
		: "=a" (result)
		: "a" (a), "b" (b)
		:
	);

	return result;
}

int main()
{
	printf("result: %i\n", add(1, 5));
	return 0;
}
