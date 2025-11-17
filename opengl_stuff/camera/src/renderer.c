#include "renderer.h"
#include "shaders.h"
#include "state.h"
#include "texture.h"

#include <GL/glew.h>
#include <cglm/cglm.h>
#include <cglm/util.h>
#include <cglm/vec3.h>
#include <stdio.h>

#include "stb_image.h"

GLuint shader_program, VBO, VAO, EBO;

texture_t container;
texture_t face;

vec3 cube_positions[] = {
	{ 0.0f,  0.0f,  0.0f},
	{ 2.0f,  5.0f, -15.0f},
	{ -1.5f, -2.2f, -2.5f },
	{ -3.8f, -2.0f, -12.3f },
	{ 2.4f, -0.4f, -3.5f },
	{ -1.7f,  3.0f, -7.5f },
	{ 1.3f, -2.0f, -2.5f },
	{ 1.5f,  2.0f, -2.5f},
	{ 1.5f,  0.2f, -1.5f},
	{ -1.3f,  1.0f, -1.5f}
};

int start_renderer()
{
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	return 1;
}

void compile_shaders()
{
	// shaders for text
	if (!create_shader_program(&shader_program,
		"shaders/vertex.glsl",
		"shaders/frag_texture.glsl"))
	{
		fprintf(stderr, "Found shader error...");
	}
}

void setup_buffers()
{
    float vertices[] = {
		// positions						// texture coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first
    glBindVertexArray(VAO);

	// bind/fill data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	// unbind (DO NOT UNBIND EBO WHILE VAO IS ACTIVE)
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

	if (!load_texture("assets/container.jpg", &container, GL_RGB)) {
		return;
	}
	
	stbi_set_flip_vertically_on_load(1);
	if (!load_texture("assets/awesomeface.png", &face, GL_RGBA)) {
		return;
	}

	glUseProgram(shader_program);
	shader_set_int(shader_program, "texture1", 0);
	shader_set_int(shader_program, "texture2", 1);
}

vec3 camera = {0.0f, 0.0f, 3.0f};
vec3 camera_front = {0.0f, 0.0f, -1.0f};
vec3 camera_up = {0.0f, 1.0f, 0.0f};

// mouse mointer coords
int prev_x = 400, prev_y = 300;
float yaw = -90.0f, pitch = 0.0f;

void process_input(state_t *state)
{
	// walking
	const float camera_speed = 0.05f;
	if (state->keys.w) {
		glm_vec3_muladds(camera_front, camera_speed, camera);
	}
	if (state->keys.s) {
		glm_vec3_mulsubs(camera_front, camera_speed, camera);
	}
	//printf("position: x: %.2f, y:%.2f, z: %.2f\n", camera[0], camera[1], camera[2]);

	vec3 strafe_axis;
	glm_cross(camera_front, camera_up, strafe_axis);
	glm_normalize(strafe_axis);

	if (state->keys.a) {
		glm_vec3_mulsubs(strafe_axis, camera_speed, camera);
	}
	if (state->keys.d) {
		glm_vec3_muladds(strafe_axis, camera_speed, camera);
	}
}

void process_mouse(state_t *state)
{
	// looking
	float mouse_x_d = state->mouse_x - prev_x;
	float mouse_y_d = prev_y - state->mouse_y;

	float sensitivity = 0.1f;
	mouse_x_d *= sensitivity;
	mouse_y_d *= sensitivity;

	yaw += mouse_x_d;
	pitch += mouse_y_d;

	if(pitch > 89.0f)
		pitch =  89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
	//printf("pitch: %.2f, %.f\n", pitch, yaw);

	vec3 direction;
	direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
	direction[1] = sin(glm_rad(pitch));
	direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
	glm_normalize(direction);
	//printf("direction: x: %.2f, y:%.2f, z: %.2f\n", direction[0], direction[1], direction[2]);

	glm_vec3_copy(direction, camera_front);
}

void render(float time, state_t *state)
{
	// clear the screen
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, container.texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, face.texture);

	glUseProgram(shader_program);
	process_input(state);
	process_mouse(state);

	mat4 projection = GLM_MAT4_IDENTITY_INIT;
	glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, &projection[0][0]);

	mat4 view = GLM_MAT4_IDENTITY_INIT;
	vec3 tmp;
	glm_vec3_add(camera_front, camera, tmp);
	glm_lookat(camera, tmp, camera_up, view);
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, &view[0][0]);

	glBindVertexArray(VAO);

	for (unsigned int i = 0; i < 10; i++)
	{
		mat4 model = GLM_MAT4_IDENTITY_INIT;
		glm_translate(model, cube_positions[i]);

		float angle = 20.0f * i;
		glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// not really needed
	glBindVertexArray(0);
}

void close_renderer()
{
	// delete opengl things
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader_program);
	destroy_texture(&container);
	destroy_texture(&face);
}
