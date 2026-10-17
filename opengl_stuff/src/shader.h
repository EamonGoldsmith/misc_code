#ifndef SHADER_H_
#define SHADER_H_

#include "light.h"
#include "material.h"
#include "texture.h"

#include <cglm/cglm.h>

#define FOR_SHADER_UNIFORM_TYPES(DO) \
	DO(int, ...) \
	DO(float, ...) \
	DO(vec3, ...) \
	DO(mat4, ...) \
	DO(light_t, ...) \
	DO(material_t, ...) \
	DO(texture_t, ...)

// creation
int create_shader_program(
	unsigned int *shader, char *vertex_file, char *frag_file);
int compile_shader_list(
	char **srcs, unsigned int num_srcs, unsigned int shader);
int compile_shader_program(int program, int *shaders, int num_shaders);
char* read_shader_file(char* filename);

// attributes
void shader_set_int(unsigned int shader, const char *name, int value);
void shader_set_float(unsigned int shader, const char *name, float value);
void shader_set_vec3(unsigned int shader, const char *name, vec3 value);
void shader_set_mat4(unsigned int shader, const char *name, mat4 value);

void shader_set_light_t(unsigned int shader, char *name, light_t* light);
void shader_set_texture_t(unsigned int shader, char *name, texture_t* texture);
void shader_set_material_t(unsigned int shader, char *name, material_t* material);

void shader_unsupported_uniform(unsigned int shader, char *name, void *value);

#define MAKE_UNIFORM_FUNC_DEF(type, ...) type: shader_set_##type,
#define shader_set_uniform(shader, name, property) \
	_Generic((property), \
		FOR_SHADER_UNIFORM_TYPES(MAKE_UNIFORM_FUNC_DEF) \
		default: shader_unsupported_uniform \
	)(shader, name, &(property))

void destroy_shader(unsigned int shader);

#endif // SHADER_H_
