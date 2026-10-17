#include "model.h"
#include "model_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int load_mesh(mesh_t *mesh, tinyobj_shape_t shape, tinyobj_attrib_t attrib)
{
	assert(mesh != NULL);

	printf("Mesh loading: %s\n", shape.name);

	mesh->len_vertices = 0;

	for (int i = shape.face_offset; i < shape.length + shape.face_offset; i++) {
		assert(attrib.face_num_verts[i] == 3);
		mesh->len_vertices += attrib.face_num_verts[i];
	}
	
	mesh->len_indices = mesh->len_vertices;

	mesh->vertices = calloc(mesh->len_vertices, sizeof(vertex_t));
	assert(mesh->vertices != NULL);

	mesh->indices = calloc(mesh->len_indices, sizeof(unsigned int));
	assert(mesh->indices != NULL);

	size_t vertex_counter = 0;
	size_t index_counter = 0;

	for (size_t i = shape.face_offset; i < shape.length + shape.face_offset; i++)
	{
		for (int v = 0; v < attrib.face_num_verts[i]; v++)
		{
			vertex_t *vertex = &mesh->vertices[vertex_counter];
			tinyobj_vertex_index_t face = attrib.faces[i * 3 + v];

			// copy position
			int idx = face.v_idx * 3;
			vertex->position[0] = attrib.vertices[idx];
			vertex->position[1] = attrib.vertices[idx + 1];
			vertex->position[2] = attrib.vertices[idx + 2];

			// copy normal
			idx = face.vn_idx * 3;
			vertex->normal[0] = attrib.normals[idx];
			vertex->normal[1] = attrib.normals[idx + 1];
			vertex->normal[2] = attrib.normals[idx + 2];

			// copy texcoord
			idx = face.vt_idx * 2;
			vertex->tex_coords[0] = attrib.texcoords[idx];
			vertex->tex_coords[1] = attrib.texcoords[idx + 1];
			vertex->tex_coords[2] = 0.0f;

			int existing_index = -1;
			for (size_t j = 0; j < vertex_counter; j++) {
				if (vertex_eq(&mesh->vertices[j], vertex)) {
					existing_index = (int)j;
					break;
				}
			}

			if (existing_index != -1) {
				mesh->indices[index_counter++] = (unsigned int)existing_index;
			} else {
				mesh->vertices[vertex_counter] = *vertex;
				mesh->indices[index_counter++] = (unsigned int)vertex_counter;
				vertex_counter++;
			}
		}
	}

	return 1;
}

void load_file(
	void *ctx, const char *path,
	const int is_mtl, const char *obj_filename, 
	char **buffer, size_t *len)
{
    FILE *fp = fopen(path, "r");

    if (!fp) {
		*len = 0;
		fprintf(stderr, "Failed to open file: %s\n", path);
		return;
	}

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	rewind(fp);

	*buffer = malloc(sizeof(char) * (file_size + 1));
	if (!buffer) {
		*len = 0;
		fprintf(stderr, "Failed to allocate file buffer\n");
		return;
	}

	size_t bytes_read = fread(*buffer, sizeof(char), file_size, fp);
	(*buffer)[file_size] = '\0';

	if (file_size != bytes_read) {
		free(*buffer);
		*buffer = NULL;
	}

	fclose(fp);
	*len = bytes_read;
	printf("Loaded file %s, %lu bytes read\n", path, bytes_read);
}

model_t model_load(const char *path)
{
	tinyobj_shape_t *shape = NULL;
	unsigned long num_shapes;

	tinyobj_material_t *material = NULL;
	unsigned long num_materials;
	
	tinyobj_attrib_t attrib;

	int result = tinyobj_parse_obj(
		&attrib,
		&shape, &num_shapes,
		&material, &num_materials,
		path, 
		load_file, 
		NULL,
		TINYOBJ_FLAG_TRIANGULATE);

	if (result != TINYOBJ_SUCCESS) {
		fprintf(stderr, "Failed to load model: %s\n", path);
		return (model_t) { NULL, 0, NULL };
	}

	assert(num_shapes > 0);

	model_t loaded = {0};
	loaded.path = strdup(path);
	loaded.meshes = calloc(num_shapes, sizeof(mesh_t));

	assert(loaded.meshes != NULL);

	printf("Model loading %li shapes\n", num_shapes);
	for (int i = 0; i < num_shapes; i++) {
		load_mesh(&loaded.meshes[i], shape[i], attrib);
	}
	loaded.len_meshes = num_shapes;

	if (num_materials > 0) {
		printf("Model loading %li materials\n", num_materials);
		for (int i = 0; i < num_materials; i++) {
			
		}
	} else {
		printf("Model: no materials found loading default\n");
	}

	tinyobj_attrib_free(&attrib);

	if (shape)
		tinyobj_shapes_free(shape, num_shapes);

	if (material)
		tinyobj_materials_free(material, num_materials);

	return loaded;
}

int model_setup(model_t *model)
{
	for (int i = 0; i < model->len_meshes; i++) {
		mesh_setup(&model->meshes[i]);
	}

	return 1;
}

void model_draw(model_t *model)
{
	
}

void destroy_model(model_t *model)
{
	assert(model != NULL);
}
