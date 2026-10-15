#ifndef LIGHT_H_
#define LIGHT_H_

#include <cglm/cglm.h>

#define FOR_LIGHT_PROPERTIES(DO) \
	DO(vec3, position, ...) \
	DO(float, constant, ...) \
	DO(float, linear, ...) \
	DO(float, quadratic, ...) \
	DO(vec3, ambient, ...) \
	DO(vec3, diffuse, ...) \
	DO(vec3, specular, ...) 

#define MAKE_STRUCT_MEMBER(type, member, ...) \
	type member;

typedef struct
{
	FOR_LIGHT_PROPERTIES(MAKE_STRUCT_MEMBER)
} light_t;

light_t *light_create(vec3 position);

void light_bind(unsigned int program, char *name, light_t* light);

/* bind multiple lights to shader by appending [i] to the name */
void light_array_bind( unsigned int program, char *name, unsigned int num,
	light_t* light);

#endif // LIGHT_H_
