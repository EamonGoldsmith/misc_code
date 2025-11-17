#include <stdio.h>

int add(int a, int b)
{
	return a + b;
}

int mul(int a, int b)
{
	return a * b;
}

int main()
{
	int (*func_op)(int, int) = NULL;
	func_op = &add;
	func_op = &mul;

	printf("result: %i\n", func_op(1, 3));

	return 0;
}
