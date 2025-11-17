#ifndef VEC_H_
#define VEC_H_

typedef struct {
    int x, y;
} ivec2d;

typedef struct {
    int x, y, z;
} ivec3d;

typedef struct {
    float x, y, z;
} fvec3d;

ivec2d ivec2d_add(ivec2d, ivec2d);
ivec3d ivec3d_add(ivec3d, ivec3d);

void print_ivec2d(ivec2d);
void print_ivec3d(ivec3d);

#endif // VEC_H_
