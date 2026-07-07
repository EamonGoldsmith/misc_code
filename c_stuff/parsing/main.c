#include <stdio.h>
#include <stdint.h>
#include <math.h>

/*
	what I really want is to read through the file and find a total file size,
	allocate space for all the data in the whole file at once. Then I can free the whole lot at once
	at program exit.

	I should keep all the data points consecutively in memory so they can be batch processed. The time points can be
	consecutive also, though I won't need to process them.
*/

typedef struct {
	uint32_t length
	float* data;
	uint32_t* time;
} DataSet;

int main()
{
	DataSet foo = {
		2,
		{ 1.0f, 2.0f },
		{ 0.0f, 0.1f }
	};

	return 0;
}
