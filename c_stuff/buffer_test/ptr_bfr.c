#include <stdio.h>

//int data[] = {1, 2, 3, 4, 5};
char *text = "Hello, world!\n";
void *el;

void func_func_set(void *d, int i)
{
	func_set(d, i);
}

void func_set(void *d, int i)
{
	// set the value d points to, to an number in the array
	*(void**)d = &text[i];
}

int main()
{
	// print addresses
	for (int i = 0; i < 5; i++) {
		printf("%c is at: \t\t%p\n", text[i], &text[i]);
	}

	func_func_set(&el, 2);

	printf("el is at: \t\t%p\n", &el);
	printf("el points to: \t\t%p\n", el);
	printf("el has value: \t\t%s\n", el);
	
	return 0;
}
