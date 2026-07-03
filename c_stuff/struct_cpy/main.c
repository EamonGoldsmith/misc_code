#include <stdio.h>
#include <string.h>

typedef struct {
	float a, b, c;
} thing;

thing thing_list[5];

int main()
{
	thing template = {
		.a = 1.0f,
		.b = 2.0f,
		.c = 3.0f,
	};

	printf("unset: %f\n", thing_list[0].b);

	for (int i = 0; i < 5; i++) {
		memcpy(&thing_list[i], &template, sizeof(thing));
	}

	printf("set to template: %f\n", thing_list[4].b);

	return 0;
}
