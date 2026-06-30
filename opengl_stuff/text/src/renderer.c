#include "vec.h"
#include "mat.h"

#include "renderer.h"
#include "text.h"
#include "shaders.h"

#include <GL/glew.h>
#include <string.h>

GLuint shader_program, VBO, VAO;
GLuint text_shader;

int start_renderer()
{
	// load a font for drawing
	if (!load_font_face()) {
		return -1;
	}

	return 1;
}

void compile_shaders()
{
	// set blend mode to make it look pretty
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// shaders for text
	create_shader_program(&text_shader,
			"shaders/font_vertex.glsl",
			"shaders/font_frag.glsl");

	float identity_matrix[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,  // Column 1
    0.0f, 1.0f, 0.0f, 0.0f,  // Column 2
    0.0f, 0.0f, 1.0f, 0.0f,  // Column 3
    0.0f, 0.0f, 0.0f, 1.0f   // Column 4
	};

	// set shader attributes
	mat4f ortho = ortho_project(0.0f, 800.0f, 600.0f, 0.0f, 0.1f, 100.0f);
	glUseProgram(text_shader);
	glUniformMatrix4fv(glGetUniformLocation(text_shader, "projection"), 1, GL_FALSE, identity_matrix);
	glUniform1i(glGetUniformLocation(text_shader, "text"), 0);
}

void setup_buffers()
{
	// setup vertex buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glEnableVertexAttribArray(0);
}

void render()
{
	// clear the screen
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// render text
	fvec3d pos = {0.0, 0.0, 80.0};
	fvec3d color = {1.0f, 0.0f, 0.0f};

	render_character(text_shader, 'A',
		pos,
		color,
		VAO, VBO);
}

void close_renderer()
{
	// delete opengl
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader_program);
	glDeleteProgram(text_shader);
}
