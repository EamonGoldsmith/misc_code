#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_BUFFER_LEN (100U)

typedef struct {
	double x; // current measurement estimate
	double P; // variance of measurement
	double R; // measurement error
	double Q; // movement error
} kalman_filter;

void kalman_update(kalman_filter *f, double z)
{
	f->x = ((f->P*z) + (f->x*f->R)) / (f->P + f->R);
	f->P = 1.0 / ((1.0/f->P) + (1.0/f->R));
}

void kalman_predict(kalman_filter *f, double u)
{
	f->x += u;
	f->P += f->Q;
}

void fill_random(double *buffer, int len, int min, int max)
{
	srand(time(NULL));

	for (int i = 0; i < len; i++) {
		buffer[i] = 50 - (double)((rand() % max) + min);
	}
}

void test_filter(double noise_scale, double movement_error)
{
	double variance[DATA_BUFFER_LEN];
	double filtered_data[DATA_BUFFER_LEN];

	// create test data
	double test_data[DATA_BUFFER_LEN];
	fill_random(test_data, DATA_BUFFER_LEN, 0, 10);

	kalman_filter my_filter = {
		.x = 50,
		.P = 1000,
		.R = noise_scale,
		.Q = movement_error,
	};
	
	// apply kalman filter
	for (int i = 0; i < DATA_BUFFER_LEN; i++) {
		kalman_predict(&my_filter, 0);

		double z = test_data[i];

		variance[i] = my_filter.P;

		kalman_update(&my_filter, z);

		filtered_data[i] = my_filter.x;
	}
	
	// show results
	for (int i = 0; i < DATA_BUFFER_LEN; i++) {
		printf("%i: noisy: %.2f, filtered: %.2f, variance: %.2f\n",
				i, test_data[i], filtered_data[i], variance[i]);
	}
}

int main()
{
	test_filter(1000, 0.1);

	return 0;
}
