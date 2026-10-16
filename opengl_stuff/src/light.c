#include "light.h"
#include "shader.h"

#include <string.h>
#include <stdlib.h>

light_t *light_create(vec3 position)
{
	light_t *light = malloc(sizeof(light_t));

	if (light == NULL) {
		return NULL;
	}

	glm_vec3_copy(light->position, position);

	// default values
	light->constant = 0.5f;
	light->linear = 0.09f;
	light->quadratic = 0.032f;

	vec3 ambient = {0.1f, 0.1f, 0.1f};
	vec3 diffuse = {0.2f, 0.2f, 0.2f};
	vec3 specular = {1.0f, 1.0f, 1.0f};
	glm_vec3_copy(light->ambient, ambient);
	glm_vec3_copy(light->diffuse, diffuse);
	glm_vec3_copy(light->specular, specular);

	return light;
}

void light_bind(unsigned int program, char *name, light_t* light)
{
	char *position = strcat(name, ".position");
	shader_set_vec3(program, position, light->ambient);
	free(position);

	shader_set_float(program, "pointLights[0].constant", light->constant);
	shader_set_float(program, "pointLights[0].linear", light->linear);
	shader_set_float(program, "pointLights[0].quadratic", light->quadratic);

	char *diffuse = strcat(name, ".diffuse");
	shader_set_vec3(program, diffuse, light->diffuse);
	free(diffuse);

	char *specular = strcat(name, ".specular");
	shader_set_vec3(program, specular, light->specular);
	free(specular);
}

void light_array_bind( unsigned int program, char *name, unsigned int num,
	light_t* light)
{

}

