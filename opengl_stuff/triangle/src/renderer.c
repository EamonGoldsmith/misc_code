#include "renderer.h"
#include "shaders.h"

#include <GL/glew.h>
#include <cglm/types.h>
#include <stdio.h>

#include <cglm/cglm.h>

GLuint shader_program, VBO, VAO, EBO;

int start_renderer()
{
	glViewport(0, 0, 800, 600);
	return 1;
}

void compile_shaders()
{
	// shaders for text
	if (!create_shader_program(&shader_program,
		"shaders/vertex.glsl",
		"shaders/frag_fancy.glsl"))
	{
		fprintf(stderr, "found shader error...");
	}

	shader_set_float(shader_program, "iTime", 0.0);
}

void setup_buffers()
{
    float vertices[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
    };

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first
    glBindVertexArray(VAO);

	// bind/fill data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	// unbind (DO NOT UNBIND EBO WHILE VAO IS ACTIVE)
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void render()
{
	// clear the screen
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	static float time;

	shader_set_float(shader_program, "iTime", time);

	// render triangle
	glUseProgram(shader_program);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	time += 0.01f;
}

void close_renderer()
{
	// delete opengl things
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader_program);
}
