#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <cglm/cglm.h>

typedef struct
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material_t;

#endif // MATERIAL_H_