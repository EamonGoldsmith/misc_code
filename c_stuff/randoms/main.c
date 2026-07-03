#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int seed_num = time(NULL);
	printf("seed is: %i\n", seed_num);

	srand48(seed_num);

	for (int i = 0; i < 20; i++) {
		float r = (float)drand48();
		printf("random number is: %.30f\n", r);
	}

	return 0;
}
