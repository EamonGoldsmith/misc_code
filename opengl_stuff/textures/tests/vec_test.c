#include "vec.h"
#include <stdio.h>

int main()
{	
	// vector add
	ivec3d a = {1, 1, 1};
	ivec3d b = {2, 2, 2};

	ivec3d c = ivec3d_add(a, b);

	print_ivec3d(c);

	return 0;
}
