#include <stdio.h>
#include <stdint.h>

float float_operation(float a, float b) {return a * b;}

int main()
{
	printf("This is a test of generics!\n");

	float res_float = float_operation(1.2f, 3.4f);

	printf("result: %f\n", res_float);

	return 0;
}
