#ifndef MAT_H_
#define MAT_H_

typedef struct {
	float m[16];
} mat4f;

mat4f ortho_project(
	float left, float right,
	float bottom, float top,
	float near, float far);

#endif // MAT_H_