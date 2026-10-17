#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <cglm/cglm.h>

#define FOR_MATERIAL_FIELDS(DO) \
	DO(vec3, ambient, ...) \
	DO(vec3, diffuse, ...) \
	DO(vec3, emission, ...) \
	DO(float, shininess, ...)

#define MAKE_MATERIAL_STRUCT_MEMBER(type, field, ...) type field;

typedef struct
{
	FOR_MATERIAL_FIELDS(MAKE_MATERIAL_STRUCT_MEMBER)
} material_t;

material_t create_material(
	vec3 ambient, vec3 diffuse, vec3 specular, float shine);

#endif // MATERIAL_H_
