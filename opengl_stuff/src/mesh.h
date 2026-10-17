#ifndef MESH_H_
#define MESH_H_

#include "texture.h"

#include <cglm/cglm.h>

typedef struct
{
	vec3 position;
	vec3 normal;
	vec3 tex_coords;
} vertex_t;

int vertex_eq(vertex_t *a, vertex_t *b);

typedef struct
{
	vertex_t *vertices;		size_t len_vertices;
	texture_t *textures;	size_t len_textures;
	unsigned int *indices;	size_t len_indices;

	unsigned int VAO, VBO, EBO;
} mesh_t;

mesh_t mesh_create( vertex_t *vertices, size_t len_vertices, texture_t *textures,
	size_t len_textures, int *indices, size_t len_indices);
void mesh_setup(mesh_t *mesh);
void mesh_draw(mesh_t *mesh, unsigned int shader);
void mesh_destroy(mesh_t *mesh);

#endif // MESH_H_
