#include "model.h"
#include "model_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

mesh_t *load_mesh(tinyobj_shape_t shape, tinyobj_attrib_t attrib)
{
	printf("Mesh loading: %s\n", shape.name);
	printf("Mesh loading %i faces\n", attrib.num_faces);
	printf("Mesh loading %i face verts\n", attrib.num_face_num_verts);

	// for each vertex of each face make a vertex_t
	// some faces may have more than three vertexes? (teapot doesn't)
	int num_vertex = 0;
	for (int i = 0; i < attrib.num_face_num_verts; i++) {
		num_vertex += attrib.face_num_verts[i];
	}

	mesh_t mesh = {0};
	mesh.len_vertices = num_vertex;

	printf("Mesh allocating %i vertices\n", num_vertex);
	mesh.vertices = calloc(num_vertex, sizeof(vertex_t));

	for (int i = shape.face_offset; i < shape.length; i += attrib.face_num_verts[i])
	{
		// face_num_verts will always be 3, if mesh triangulated
		for (int j = 0; j < attrib.face_num_verts[i]; j++)
		{
			assert(attrib.face_num_verts[i] == 3);

			vertex_t *vertex = &mesh.vertices[i + j];

			// copy position
			const int vertex_idx = attrib.faces[j + i].v_idx;
			vertex->position[j] = attrib.vertices[vertex_idx];
			vertex->position[j] = attrib.vertices[vertex_idx + 1];
			vertex->position[j] = attrib.vertices[vertex_idx + 2];

			// copy normal
			const int normal_idx = attrib.faces[j + i].vn_idx;
			vertex->normal[j] = attrib.normals[normal_idx];
			vertex->normal[j] = attrib.normals[normal_idx + 1];
			vertex->normal[j] = attrib.normals[normal_idx + 2];

			// copy texcoord
			const int texcoord_idx = attrib.faces[j + i].vt_idx;
			vertex->tex_coords[j] = attrib.texcoords[texcoord_idx];
			vertex->tex_coords[j] = attrib.texcoords[texcoord_idx + 1];
			vertex->tex_coords[j] = attrib.texcoords[texcoord_idx + 2];
		}
	}

	return NULL;
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

	model_t loaded = {0};
	loaded.path = strdup(path);
	loaded.meshes = malloc(sizeof(mesh_t*) * num_shapes);

	for (int i = 0; i < num_shapes; i++) {
		loaded.meshes[i] = load_mesh(shape[i], attrib);
	}
	loaded.len_meshes = num_shapes;

	for (int i = 0; i < num_materials; i++) {
		printf("Skip\n");
	}

	tinyobj_attrib_free(&attrib);

	if (shape)
		tinyobj_shapes_free(shape, num_shapes);

	if (material)
		tinyobj_materials_free(material, num_materials);

	return loaded;
}

