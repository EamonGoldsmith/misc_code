#include "shaders.h"

#include <GL/glew.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFO_LOG_LEN (512U)
char info_log[INFO_LOG_LEN];

int create_shader_program(unsigned int *shader, char *vertex_file, char *frag_file)
{
	// create
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	// load
	char *vertex_src = read_shader_file(vertex_file);
	char *frag_src = read_shader_file(frag_file);

	//printf("vertex code: \n%s\n", vertex_src);
	//printf("fragment code: \n%s\n", frag_src);

	// compile
	if (!compile_shader_list(&vertex_src, 1, vertex_shader)) {
		return -1;
	}
	if (!compile_shader_list(&frag_src, 1, frag_shader)) {
		return -1;
	}

	printf("Linking shader program\n");

	// link
	*shader = glCreateProgram();
	GLint shader_list[] = { vertex_shader, frag_shader };
	if (!compile_shader_program(*shader, &shader_list[0], 2)) {
		return -1;
	}

	return 1;
}

int compile_shader_list(char **srcs, unsigned int num_srcs,
		unsigned int shader)
{
	GLint success;

	glShaderSource(shader, num_srcs, (const char**)srcs, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, INFO_LOG_LEN, NULL, info_log);
		fprintf(stderr, "Shader Compilation Error:\n%s", info_log);
	}

	for (int i = 0; i < num_srcs; i++) {
		free(srcs[i]);
	}

	return success;
}

int compile_shader_program(int program, int *shaders, int num_shaders)
{
	int success;

	// attach shaders
	for (int n = 0; n < num_shaders; n++) {
		glAttachShader(program, shaders[n]);
	}

	// link program
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(program, INFO_LOG_LEN, NULL, info_log);
		fprintf(stderr, "Shader Linking Error:\n%s\n", info_log);
	}

	// delete shaders
	for (int n = 0; n < num_shaders; n++) {
		glDeleteShader(shaders[n]);
	}

	return success;
}

char *read_shader_file(char *filename)
{
	printf("Reading shader: %s\n", filename);

	FILE *fp = fopen(filename, "r");
	if (!fp) {
		fprintf(stderr, "Could not open file %s, reason: %s\n",
			filename, strerror(errno));
		return NULL;
	}

	// seek to end to get file size
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	rewind(fp);

	// allocate buffer (+1 for terminator)
	char *buffer = (char*)malloc((length + 1) * sizeof(char));
	if (!buffer) {
		fprintf(stderr, "Memory allocation failed for:%s\n", filename);
		fclose(fp);
		return NULL;
	}

	// read file into buffer
	unsigned int read_size = fread(buffer, 1, length, fp);
	buffer[read_size] = 0;

	fclose(fp);
	return buffer;
}

void shader_set_int(unsigned int shader, const char *name, int value)
{
	glUniform1i(glGetUniformLocation(shader, name), value);
}

void shader_set_float(unsigned int shader, const char *name, float value)
{
	glUniform1f(glGetUniformLocation(shader, name), value);
}

void destroy_shader(unsigned int shader)
{
	glDeleteProgram(shader);
}
