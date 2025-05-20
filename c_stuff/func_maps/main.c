#include <stdio.h>

enum types {
	A, B, C, G
};

struct babel_msg {
	enum types type;
	char data[8];
};

typedef int (*func_sig) (struct babel_msg);

struct map_element
{
	enum types pair;
	func_sig ptr;
};

void generic()
{
	printf("command not matched");
}

int search_elements(struct map_element elements[], struct babel_msg msg)
{
	int matched = 0;
	for (int i = 0; i < 4; i++) {
		if (elements[i].pair == msg.type) {
			elements[i].ptr(msg);
			matched = 1;
			return matched;
		}
	}

	if (!matched) {
		generic();
	}

	return 0;
}


int do_some_foo(struct babel_msg msg)
{
	for (int i = 0; i < 8; i++) {
		printf("%c, ", msg.data[i]);
	}
	printf("\n");
}

int do_some_other_foo(struct babel_msg msg) {
	printf("some other\n");
}

int new_function(struct babel_msg msg) {
	printf("NEW\n");
}

int main()
{
	struct babel_msg new_msg = {G, {'M', 'E', 'S', 'S', 'A', 'G', 'E', '!'}};

	struct map_element map_elements[] = {
		{A, do_some_foo},
		{B, do_some_other_foo},
		{C, new_function}
	};

	search_elements(map_elements, new_msg);
}
