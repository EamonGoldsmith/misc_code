#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <cglm/cglm.h>

#define FOR_MATERIAL_FIELDS(DO) \
	DO(ambient, ...) \
	DO(specular, ...) \
	DO(emission, ...) \

#define DEFINE_ENUM(field, ...) MATERIAL_##field,

enum material_fields {
	FOR_MATERIAL_FIELDS(DEFINE_ENUM)
};

#define SWITCH_CASE(field, ...) case MATERIAL_##field: return #field;

typedef struct
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material_t;

material_t create_material(
	vec3 ambient, vec3 diffuse, vec3 specular, float shine);

#endif // MATERIAL_H_
