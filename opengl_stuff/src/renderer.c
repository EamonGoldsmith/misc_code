#include "renderer.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "state.h"
#include "mesh.h"
#include "model.h"

#include <GL/glew.h>
#include <cglm/cglm.h>

#include <stdio.h>

#include "vertex_data.h"

GLuint pgm, cubeVBO, cubeVAO;
GLuint light_source, lightVAO;
camera_t camera;

texture_t diffuse_map;
texture_t specular_map;
texture_t emission_map;

model_t teapot;

int start_renderer(const int width, const int height)
{
	// setup some opengl stuff
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	// create camera
	init_camera(&camera);

	// load teapot model:
	//teapot = model_load("assets/utah_teapot.obj");

	/*
	for (int i = 0; i < teapot.meshes[0]->len_vertices; i++) {
		printf("V: %f\n", teapot.meshes[0]->vertices[i].position[0]);
	}
	*/

	return 1;
}

void compile_shaders()
{
	// shaders for container
	if (!create_shader_program(&pgm,
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
	if (!load_texture("assets/crate_diffuse.png",
			&diffuse_map, GL_RGBA, TEXTURE_DIFFUSE))
	{
		fprintf(stderr, "failed to load crate texture\n");
	}

	if (!load_texture("assets/crate_specular.png",
			&specular_map, GL_RGBA, TEXTURE_SPECULAR)) 
	{
		fprintf(stderr, "failed to load spec texture\n");
	}

	if (!load_texture("assets/matrix.jpg",
			&emission_map, GL_RGB, TEXTURE_EMISSION))
	{
		fprintf(stderr, "failed to load mat texture\n");
	}
}

void setup_buffers()
{
	// create buffers for container, create buffers on gpu with indexes
    glGenVertexArrays(1, &cubeVAO);

	glGenBuffers(1, &cubeVBO);

	// bind/fill data, moves static array into gpu memory
	extern float vertices[]; // from vertex_data.h
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind the container VAO first
    glBindVertexArray(cubeVAO);

	// position attribute of VAO (first three nums)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	// normal attribute of VAO (next three nums, normals of faces)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture attribute (what corners map to where on the texture)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// create buffers for point light (floating cube thing)
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	
	// cube and light are both cubes so re-use buffer
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	// light position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/*
	// CREATE THE TEAPOT MESH
	//mesh_setup(teapot.meshes[0]);
	mesh_t = teapot.meshes[0];

	glGenVertexArrays(1, &tp->VAO);
	glGenBuffers(1, &tp->VBO);

    glBindBuffer(GL_ARRAY_BUFFER, tp->VBO);
    glBufferData(GL_ARRAY_BUFFER, tp->len_vertices, tp->vertices, GL_STATIC_DRAW);

    // bind the container VAO first
    glBindVertexArray(tp->VAO);

	// position attribute of VAO (first three nums)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),	(void*)0);
    glEnableVertexAttribArray(0);

	// normal attribute of VAO (next three nums, normals of faces)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture attribute (what corners map to where on the texture)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	*/
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

	// bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse_map.id);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular_map.id);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emission_map.id);

	// render the crates
	glUseProgram(pgm);

	// set light properties
	shader_set_vec3(pgm, "pointLights[0].position", &light_positions[0][0]);
	shader_set_float(pgm, "pointLights[0].constant", 0.1f);
	shader_set_float(pgm, "pointLights[0].linear", 0.09f);
	shader_set_float(pgm, "pointLights[0].quadratic", 0.032f);
	shader_set_vec3(pgm, "pointLights[0].ambient", (vec3){1.0f, 1.0f, 1.0f});
	shader_set_vec3(pgm, "pointLights[0].diffuse", (vec3){0.2f, 0.2f, 0.2f});
	shader_set_vec3(pgm, "pointLights[0].specular", (vec3){1.0f, 1.0f, 1.0f});

	shader_set_vec3(pgm, "pointLights[1].position", &light_positions[1][0]);
	shader_set_float(pgm, "pointLights[1].constant", 0.1f);
	shader_set_float(pgm, "pointLights[1].linear", 0.09f);
	shader_set_float(pgm, "pointLights[1].quadratic", 0.032f);
	shader_set_vec3(pgm, "pointLights[1].ambient", (vec3){0.5f, 0.5f, 0.5f});
	shader_set_vec3(pgm, "pointLights[1].diffuse", (vec3){0.2f, 0.2f, 0.2f});
	shader_set_vec3(pgm, "pointLights[1].specular", (vec3){1.0f, 1.0f, 1.0f});

	shader_set_vec3(pgm, "pointLights[2].position", &light_positions[2][0]);
	shader_set_float(pgm, "pointLights[2].constant", 0.1f);
	shader_set_float(pgm, "pointLights[2].linear", 0.09f);
	shader_set_float(pgm, "pointLights[2].quadratic", 0.032f);
	shader_set_vec3(pgm, "pointLights[2].ambient", (vec3){0.5f, 0.5f, 0.5f});
	shader_set_vec3(pgm, "pointLights[2].diffuse", (vec3){0.2f, 0.2f, 0.2f});
	shader_set_vec3(pgm, "pointLights[2].specular", (vec3){1.0f, 1.0f, 1.0f});

	// set directional light
	shader_set_vec3(pgm, "dirLight.direction", (vec3){-2.0f, -1.0f, -0.3f});
	shader_set_vec3(pgm, "dirLight.ambient", (vec3){0.00f, 0.00f, 0.00f});
	shader_set_vec3(pgm, "dirLight.diffuse", (vec3){0.00f, 0.00f, 0.00f});
	shader_set_vec3(pgm, "dirLight.specular", (vec3){0.00f, 0.00f, 0.00f});

	// set material properties
	shader_set_int(pgm, "material.diffuse", 0);
	shader_set_int(pgm, "material.specular", 1);
	shader_set_int(pgm, "material.emission", 2);
	shader_set_float(pgm, "material.shininess", 0.8f);

	// set other uniforms
	shader_set_float(pgm, "Time", time / 10.0);
	shader_set_vec3(pgm, "viewPos", &camera.pos[0]);
	shader_set_mat4(pgm, "projection", projection);
	shader_set_mat4(pgm, "view", view);

	glBindVertexArray(cubeVAO);

	// draw the CUBES

	for (int i = 0; i < 10; i++)
	{
		glm_mat4_dup(GLM_MAT4_IDENTITY, model);
		
		extern vec3 cube_positions[];
		glm_translate(model, cube_positions[i]);

		float angle = (i * 20.0f) * (time / 3000.0f);
		glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});
		
		glUniformMatrix4fv(glGetUniformLocation(pgm, "model"),
			1, GL_FALSE, &model[0][0]);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// draw teapot

	// render the light sources
	glUseProgram(light_source);

	// set light properties
	shader_set_vec3(light_source, "light.ambient", (vec3){0.1f, 0.1f, 0.1f});
	shader_set_vec3(light_source, "light.diffuse", (vec3){1.0f, 1.0f, 1.0f});
	shader_set_vec3(light_source, "ligth.specular", (vec3){1.0f, 1.0f, 1.0f});

	shader_set_mat4(light_source, "projection", projection);
	shader_set_mat4(light_source, "view", view);

	for (int i = 0; i < 3; i++)
	{
		glm_mat4_dup(GLM_MAT4_IDENTITY, model);
		glm_scale(model, (vec3){0.2f, 0.2f, 0.2f});
		glm_translate(model, light_positions[i]);

		shader_set_mat4(light_source, "model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	// unbind
	glBindVertexArray(0);
}

void close_renderer()
{
	// delete opengl things
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteProgram(pgm);

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteProgram(light_source);

	destroy_texture(&diffuse_map);
	destroy_texture(&specular_map);
	destroy_texture(&emission_map);
}
