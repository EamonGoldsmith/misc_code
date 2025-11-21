#include "renderer.h"
#include "camera.h"
#include "shaders.h"
#include "texture.h"
#include "state.h"

#include <GL/glew.h>
#include <cglm/cglm.h>

#include <cglm/mat4.h>
#include <stdio.h>

GLuint shader_program, cubeVBO, cubeVAO;
GLuint light_source, lightVAO;
camera_t camera;

vec3 light_pos = {1.2f,1.0f, 2.0f};

texture_t diffuse_map;
texture_t specular_map;
texture_t emission_map;

#include "cube.h"

int start_renderer(const int width, const int height)
{
	// setup some opegl stuff
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	// create camera
	init_camera(&camera);

	return 1;
}

void compile_shaders()
{
	// shaders for container
	if (!create_shader_program(&shader_program,
		"shaders/phong_lighting.vs",
		"shaders/phong_lighting.fs"))
	{
		fprintf(stderr, "Found shader error...");
	}

	// shaders for light source
	if (!create_shader_program(&light_source,
		"shaders/light_cube.vs",
		"shaders/light_cube.fs"))
	{
		fprintf(stderr, "Found shader error...");
	}

	// load textures
	(void) load_texture("assets/crate_diffuse.png", &diffuse_map, GL_RGBA);
	(void) load_texture("assets/crate_specular.png", &specular_map, GL_RGBA);
	(void) load_texture("assets/matrix.jpg", &emission_map, GL_RGB);
}

void setup_buffers()
{
	// create buffers for container
    glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	// bind/fill data
	extern float vertices[];
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind the container VAO first
    glBindVertexArray(cubeVAO);

	// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// create buffers for light
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	
	// cube and light are both cubes so re-use buffer
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	// light position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void render(float time, state_t *state)
{
	// clear the screen
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update camera transforms
	camera_move(&camera, state);
	camera_look(&camera, state->mouse_x, state->mouse_y);

	// rendering transforms
	mat4 model = GLM_MAT4_IDENTITY_INIT;
	
	mat4 view = GLM_MAT4_IDENTITY_INIT;
	vec3 tmp;
	glm_vec3_add(camera.front, camera.pos, tmp);
	glm_lookat(camera.pos, tmp, camera.up, view);
	
	mat4 projection = GLM_MAT4_IDENTITY_INIT;
	glm_perspective(glm_rad(45.0f), 1066.0f / 800.0f, 0.1f, 100.0f, projection);
	
	// render the container

	// set uniforms for light shader
	glUseProgram(shader_program);

	// bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse_map.texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular_map.texture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emission_map.texture);

	// set material properties
	glUniform1i(glGetUniformLocation(shader_program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shader_program, "material.specular"), 1);
	glUniform1i(glGetUniformLocation(shader_program, "material.emission"), 2);
	glUniform1f(glGetUniformLocation(shader_program, "material.shininess"), 0.7f);

	// set light properties
	glUniform3f(glGetUniformLocation(shader_program, "light.position"), light_pos[0], light_pos[1], light_pos[2]);
	glUniform3f(glGetUniformLocation(shader_program, "light.ambient"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(shader_program, "light.diffuse"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(shader_program, "light.specular"), 1.0f, 1.0f, 1.0f);

	// set other uniforms
	glUniform1f(glGetUniformLocation(shader_program, "Time"), time/10.0);
	glUniform3f(glGetUniformLocation(shader_program, "viewPos"), camera.pos[0], camera.pos[1], camera.pos[2]);
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, &view[0][0]);

	glBindVertexArray(cubeVAO);

	for (int i = 0; i < 10; i++) {
		glm_mat4_dup(GLM_MAT4_IDENTITY, model);
		
		extern vec3 cube_positions[];
		vec3 offset = {cube_positions[i][0], cube_positions[i][1], cube_positions[i][2]};
		glm_translate(model, offset);

		float angle = (i * 20.0) * (time / 3000.0);
		glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});
		
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model[0][0]);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// render the light source
	glm_mat4_dup(GLM_MAT4_IDENTITY, model);

	// calculate light position
	float radius = 3.0f;
	float speed = 0.05f;
	light_pos[0] = cos(glm_rad(time * speed)) * radius;
	light_pos[2] = sin(glm_rad(time * speed)) * radius;
	glm_translate(model, light_pos);
	glm_scale(model, (vec3){0.2f, 0.2f, 0.2f});

	glUseProgram(light_source);

	// set light properties
	glUniform3f(glGetUniformLocation(light_source, "light.position"), light_pos[0], light_pos[1], light_pos[2]);
	glUniform3f(glGetUniformLocation(light_source, "light.ambient"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(light_source, "light.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(light_source, "light.specular"), 1.0f, 1.0f, 1.0f);

	glUniformMatrix4fv(glGetUniformLocation(light_source, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(light_source, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(light_source, "model"), 1, GL_FALSE, &model[0][0]);

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// not really needed
	glBindVertexArray(0);
}

void close_renderer()
{
	// delete opengl things
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteProgram(shader_program);

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteProgram(light_source);

	destroy_texture(&diffuse_map);
	destroy_texture(&specular_map);
	destroy_texture(&emission_map);
}
