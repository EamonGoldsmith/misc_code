#include <stdio.h>

int main()
{
	int x[] = { 1, 2, 3, 4, };

	printf("addr 0x%.2x\n", &x);
	printf("0x%.2x\n", &x + 1);
	printf("0x%.2x\n", x + 1);

	return 0;
}
