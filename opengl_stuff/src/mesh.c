#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "string_builder.h"

#include <GL/glew.h>

int vertex_eq(vertex_t *a, vertex_t *b)
{
	float tol = 0.0001f;
	return
		fabsf(a->position[0] - b->position[0]) < tol &&
		fabsf(a->position[1] - b->position[1]) < tol &&
		fabsf(a->position[2] - b->position[2]) < tol &&

		fabsf(a->normal[0] - b->normal[0]) < tol &&
		fabsf(a->normal[1] - b->normal[1]) < tol &&
		fabsf(a->normal[2] - b->normal[2]) < tol &&

		fabsf(a->tex_coords[0] - b->tex_coords[0]) < tol &&
		fabsf(a->tex_coords[1] - b->tex_coords[1]) < tol;
}

mesh_t mesh_create(
	vertex_t *vertices, size_t len_vertices,
	texture_t *textures, size_t len_textures,
	int *indices, size_t len_indices)
{
	return (mesh_t) {
		vertices, len_vertices,
		textures, len_textures,
		indices, len_indices
	};
}

void mesh_setup(mesh_t *mesh)
{
	assert(mesh != NULL);
	assert(mesh->vertices != NULL);
	assert(mesh->len_vertices > 0);

	glGenVertexArrays(1, &mesh->VAO);

		// create buffers
		glGenBuffers(1, &mesh->VBO);
		glGenBuffers(1, &mesh->EBO);

	glBindVertexArray(mesh->VAO);

	// fill vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER,
		mesh->len_vertices * sizeof(vertex_t), mesh->vertices, GL_STATIC_DRAW);

	// fill element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		mesh->len_indices * sizeof(int), mesh->indices, GL_STATIC_DRAW);

	// vertex array layout:

		// position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(vertex_t), (void*)offsetof(vertex_t, position));

		// normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			sizeof(vertex_t), (void*)offsetof(vertex_t, normal));

		// tex coord
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
			sizeof(vertex_t), (void*)offsetof(vertex_t, tex_coords));

	glBindVertexArray(0);
}

void mesh_draw(mesh_t *mesh, unsigned int shader)
{
	sb_t variable;
	sb_init(&variable);

	for (int i = 0; i < mesh->len_textures; i++)
	{
		// bind textures
		glActiveTexture(GL_TEXTURE0 + i);

		texture_t texture = mesh->textures[i];
		sb_appendf(&variable, "material.%s%i", texture_type_str(texture), i);
		sb_terminate(&variable);

		shader_set_int(shader, variable.str, texture.id);
		sb_clear(&variable);

		glBindTexture(GL_TEXTURE_2D, texture.id);
	}

	// draw
	glBindVertexArray(mesh->VAO);
	glDrawElements(GL_TRIANGLES, mesh->len_indices, GL_UNSIGNED_INT, 0);
}

void mesh_destroy(mesh_t *mesh)
{
	// free buffers allocated in model.c
	
	// free textures

}
