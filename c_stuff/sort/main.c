#include <stdio.h>
#include <stdlib.h>

int arr[20] = {2, 41, 13, 54, 321, 543, 26, 34, 7, 3, 21, 543, 2334, 654, 12, 23, 65, 45, 23, 65};

int compare_ints(const void *a, const void *b)
{
	const int *ia = a;
	const int *ib = b;

	return (*ia > *ib) - (*ia < *ib);
}

int main()
{
	for (int i = 0; i < 20; i++) {
		printf("%i, ", arr[i]);
	}
	printf("\n");

	int *arr_sorted = arr;
	qsort(arr_sorted, 20, sizeof *arr, compare_ints);

	for (int i = 0; i < 20; i++) {
		printf("%i, ", arr_sorted[i]);
	}
	printf("\n");

	return 0;
}
