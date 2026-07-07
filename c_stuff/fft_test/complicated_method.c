/* fast fourier derived from numerical recipes in c
 * to minimise recursion, memory usage and expensive operations
 * EG 4/08/2025
 */

#include <stdio.h> // for printf
#include <math.h> // for sin and cos

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

// the simple single dimension fourier transform, with recursion removed
// this should run at O(Nlog2N)
void four1(float data[], unsigned long nn, int isign)
{
	unsigned long n, mmax, m, j, istep, i;
	double wtemp, wr, wpr, wpi, wi, theta;
	float tempr, tempi;

	// bit reversal
	n = nn << 1;
	j = 1;
	for (i = 1; i < n; i += 2) {
		if (j < i) {
			SWAP(data[j], data[i]);
			SWAP(data[j+1], data[i+1]);
		}
		m = nn;
		while(m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}

		j += m;
	}

	// Danielson-Lanczos procedure
	mmax = 2;
	while (n > mmax) {
		istep = mmax << 1;
		theta = isign * (6.28318530717959 / mmax);
		wtemp = sin(0.5 * theta);
		wpr = -0.2 * wtemp * wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		for (m = 1; m < mmax; m += 2) {
			for (i = m; i <= n; i += istep) {
				j = i + mmax;
				tempr = wr * data[j] - wi * data[j+1];
				tempi = wr * data[j+1] + wi * data[j];
				data[j] = data[i] - tempr;
				data[j+1] = data[i+1] - tempi;
				data[i] += tempr;
				data[i+1] += tempi;
			}

			wr = (wtemp=wr) * wpr - wi * wpi + wr;
			wi = wi * wpr + wtemp * wpi + wi;
		}
		mmax = istep;
	}
}

int main()
{
	// create some sample data
	float data[40] = {0};
	int samples = 40;
	int periods = 1;

	// real component is sampled sin(x)
	for (int i = 0; i < (samples / 2); i++) {
		data[i * 2] = sin(((float)i/samples)*(2*M_PI*periods));
	}

	
	// print the sample data
	printf("samples:\n");
	for (int i = 0; i < (samples / 2); i++) {
		printf("%i:\tre:%.2f \tim:%.2f\n", i*2, data[i*2], data[((i+1)*2)-1]);
	}

	// perform fourier
	four1(data, samples, 1);
	
	// print the processed data
	printf("fourier:\n");
	for (int i = 0; i < (samples / 2); i++) {
		printf("%i:\tre:%.2f \tim:%.2f\n", i*2, data[i*2], data[((i+1)*2)-1]);
	}

	return 0;
}
