#ifndef LIGHT_H_
#define LIGHT_H_

#include <cglm/cglm.h>

typedef struct
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} light_t;

#endif // LIGHT_H_