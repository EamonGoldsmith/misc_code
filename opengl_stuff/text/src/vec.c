#include "vec.h"
#include <stdio.h>

ivec2d ivec2d_add(ivec2d a, ivec2d b)
{
	ivec2d res;
	res.x = a.x + b.x;
	res.y = a.y + b.y;

	return res;
}

ivec3d ivec3d_add(ivec3d a, ivec3d b)
{
	ivec3d res;
	res.x = a.x + b.x;
	res.y = a.y + b.y;
	res.z = a.z + b.z;

	return res;
}

void print_ivec2d(ivec2d vec)
{
	printf("x: %i, y: %i\n", vec.x, vec.y);
}

void print_ivec3d(ivec3d vec)
{
	printf("x: %i, y: %i, z: %i\n", vec.x, vec.y, vec.z);
}
