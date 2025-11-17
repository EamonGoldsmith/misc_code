#include "renderer.h"
#include "shaders.h"
#include "texture.h"

#include <GL/glew.h>
#include <stdio.h>

#include "stb_image.h"

GLuint shader_program, VBO, VAO, EBO;

texture_t bricks;
texture_t face;

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
		"shaders/frag_texture.glsl"))
	{
		fprintf(stderr, "found shader error...");
	}
}

void setup_buffers()
{
    float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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

	// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

	// unbind (DO NOT UNBIND EBO WHILE VAO IS ACTIVE)
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

	if (!load_texture("assets/wall.jpg", &bricks, GL_RGB)) {
		return;
	}
	
	stbi_set_flip_vertically_on_load(1);
	if (!load_texture("assets/awesomeface.png", &face, GL_RGBA)) {
		return;
	}

	glUseProgram(shader_program);
	shader_set_int(shader_program, "Texture1", 0);
	shader_set_int(shader_program, "Texture2", 1);
}

void render()
{
	// clear the screen
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// render triangle
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bricks.texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, face.texture);

	glUseProgram(shader_program);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void close_renderer()
{
	// delete opengl things
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader_program);
	destroy_texture(&bricks);
	destroy_texture(&face);
}
