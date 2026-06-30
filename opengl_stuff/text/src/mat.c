#include "mat.h"

#include <string.h>

mat4f ortho_project(
	float left, float right,
	float bottom, float top,
	float near, float far)
{
	mat4f result;
    
	// set all elements to 0 initially
    memset(result.m, 0, sizeof(result.m)); 

    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    float tz = -(far + near) / (far - near);

    // orthographic matrix structure (column-major order)
    //
    // col 0: (m[0], m[1], m[2], m[3])
    // col 1: (m[4], m[5], m[6], m[7])
    // col 2: (m[8], m[9], m[10], m[11])
    // col 3: (m[12], m[13], m[14], m[15])
    
    // column 0 (X axis scale & offset)
    result.m[0] = 2.0f / (right - left);
    
    // column 1 (Y axis scale & offset)
    result.m[5] = 2.0f / (top - bottom);
    
    // column 2 (Z axis scale & offset)
    result.m[10] = -2.0f / (far - near); 
    
    // column 3 (translation/homogeneous)
    result.m[12] = tx;
    result.m[13] = ty;
    result.m[14] = tz;
    result.m[15] = 1.0f; // homogeneous component

    return result;
}