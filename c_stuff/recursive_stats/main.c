#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATASET_SIZE 128U

float tof(int a)
{
	return a + 0.0f;
}

void append(float *a, float *m, float *d, float v, float s)
{
	// check maximum
	if (v > *m)
		*m = v;

	// update avg (sma algorithm)
	*a = ((*a * tof(s)) + v) / (tof(s) + 1.0f);

	// update diff
	*d += (v - *d);
}	

void refresh(float *a, float *m, float *diff)
{
	*a = *m = *diff = 0;
}

int main()
{
	float avg, max, diff;

	srand(time(0));

	// test known set
	
	refresh(&avg, &max, &diff);

	for (unsigned int i = 0; i < DATASET_SIZE; i++) {
		append(&avg, &max, &diff, 0.5f, i + 1);
	}

	printf("average: %f, maximum: %f, diff: %f\n", avg, max, diff);
	
	// test random numbers

	refresh(&avg, &max, &diff);

	for (unsigned int i = 0; i < DATASET_SIZE; i++) {
		append(&avg, &max, &diff, rand() / 10000.0f, i + 1);
	}

	printf("average: %f, maximum: %f, diff: %f\n", avg, max, diff);

	return 0;
}
