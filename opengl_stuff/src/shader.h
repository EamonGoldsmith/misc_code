#ifndef SHADER_H_
#define SHADER_H_

#include <cglm/cglm.h>

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

void destroy_shader(unsigned int shader);

#endif // SHADER_H_
