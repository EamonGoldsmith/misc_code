// cc expt.c -O2 -o expt

#include <stdio.h>

int expt(int b, int n)
{
	if (n == 0) {
		return 1;
	}
	return b * expt(b, n - 1);
}

/* tail call optimisation and power half */
int fast_expt_iter(int b, int n, int prod)
{
	if (prod == 0) {
		return prod;
	} else {
		return fast_expt_iter(b, n - 1, b * prod);
	}
}
int fast_expt(int b, int n)
{
	if (n == 0) {
		return 1;
	} else if (n % 2 == 0) {
		int tmp = fast_expt(b, n / 2);
		return tmp * tmp;
	} else {
		return fast_expt_iter(b, n, 1);
	}
}

int main(int argc, char **argv)
{
	int b, n, r;

	sscanf(argv[1], "%d", &b);
	sscanf(argv[2], "%d", &n);

	r = fast_expt(b, n);

	printf("result: %d\n", r);

	return 0;
}
