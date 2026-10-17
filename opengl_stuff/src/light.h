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

#define MAKE_LIGHT_STRUCT_MEMBER(type, member, ...) \
	type member;

typedef struct
{
	FOR_LIGHT_PROPERTIES(MAKE_LIGHT_STRUCT_MEMBER)

} light_t;

#define MAKE_UNIFORM_CACHE_MEMBER(type, member, ...) \
	int member;

typedef struct
{
	FOR_LIGHT_PROPERTIES(MAKE_UNIFORM_CACHE_MEMBER)
} light_uniforms_t;

#define INCREMENT_PER_ITEM(type, member, ...) \
	+ 1

const static inline int num_light_properties()
{
	return 0 + FOR_LIGHT_PROPERTIES(INCREMENT_PER_ITEM);
}

light_t *light_create(vec3 position);

#endif // LIGHT_H_
