#include <stdio.h>
#include <math.h>

#define PI (3.14159265)
#define BUFFER_LEN (128)
#define MAX_K 8
#define N 8

// complex number representation
typedef struct {
	double re;
	double im;
} complex;

// sized pointer type
typedef struct {
	double data[BUFFER_LEN];
	int size;
} sptr_t;

// copy n doubles from src to dst
void array_cpy(double src[], double dst[], int n)
{
	if (n > BUFFER_LEN)
		return;

	for (int i = 0; i < n; i++) {
		dst[i] = src[i];
	}
}

// Cooley Tukey recursive FFT
complex cooley_tukey_fft(complex x[])
{
	complex Xk;
	complex evenP;
	complex oddP;
	double c, s, reP, imP;
	int m, k, i;

	for (k = 0; k < MAX_K; k++) {
		double theta = (-2 * PI * k) / (N / 2);

		evenP.re = 0;
		evenP.im = 0;
		oddP.re = 0;
		oddP.im = 0;

		for (m = 0; m < (N / 2); m++) {
			c = cos(theta * (float)m);
			s = sin(theta * (float)m);

			// even
			reP = (x[2 * m].re * c) - (x[2 * m].im * s);
			evenP.re += reP;
			imP = (x[2 * m].re * s) + (x[2 * m].im * c);
			evenP.im += imP;

			// odd
			reP = (x[(2 * m) + 1].re * c) - ((x[(2 * m) + 1].im * s));		
			oddP.re += reP;
			imP = (x[(2 * m) + 1].re * s) + ((x[(2 * m) + 1].im * c));
			oddP.im = imP;
		}
	}

	Xk.re = evenP.re + (cos(theta / 2) * oddP.re) - (sin(theta / 2) * oddP.im);
	Xk.im = evenP.im + (cos(theta / 2) * oddP.im) - (sin(theta / 2) * oddP.re);

	return Xk;
}

int main()
{
	// create a sample
	complex sample[N];

	for (int i = 0; i < N; i++) {
		sample[i].re = 1;
		sample[i].im = 1;
	}

	// perform the transform on the sample
	complex res = cooley_tukey_fft(sample);

	printf("res: re %f, im %f\n", res.re, res.im);

	return 0;
}
