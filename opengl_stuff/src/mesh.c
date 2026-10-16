#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "string_builder.h"

#include <GL/glew.h>

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
	glGenVertexArrays(1, &mesh->VAO);

		// create buffers
		glGenBuffers(1, &mesh->VBO);
		glGenBuffers(1, &mesh->EBO);

	glBindVertexArray(mesh->VAO);

	// fill vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER,
		mesh->len_vertices, mesh->vertices, GL_STATIC_DRAW);

	// fill element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		mesh->len_indices, mesh->indices, GL_STATIC_DRAW);

	// vertex array layout:

		// position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(vertex_t), (void*)0);

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
		
		shader_set_float(shader, variable.str, texture.id);
		sb_clear(&variable);

		glBindTexture(GL_TEXTURE_2D, texture.id);
	}

	// draw
	glBindVertexArray(mesh->VAO);
	glDrawElements(GL_TRIANGLES, mesh->len_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void mesh_destroy(mesh_t *mesh)
{
	// free buffers allocated in model.c
	
	// free textures

}
