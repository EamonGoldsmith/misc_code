#include "model.h"
#include "model_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

mesh_t *load_mesh(tinyobj_shape_t shape, tinyobj_attrib_t attrib)
{
	printf("Mesh loading: %s\n", shape.name);

	// for each vertex of each face make a vertex_t
	// some faces may have more than three vertexes? (teapot doesn't)
	int num_vertex = 0;
	for (int i = 0; i < attrib.num_face_num_verts; i++) {
		num_vertex += attrib.face_num_verts[i];
	}

	printf("Indices of face 0: (pos %i, tex %i, norm %i)\n",
		attrib.faces[0].v_idx,
		attrib.faces[0].vt_idx,
		attrib.faces[0].vn_idx);

	printf("Mesh vertex 0: (%f, %f, %f)\n",
		attrib.vertices[0], 
		attrib.vertices[1], 
		attrib.vertices[2]);

	printf("Mesh loading %i faces\n", attrib.num_faces);
	printf("Mesh loading %i face verts\n", attrib.num_face_num_verts);
	printf("Mesh allocating %i vertices\n", num_vertex);

	mesh_t mesh = {0};
	mesh.len_vertices = num_vertex;

	// each mesh has separate allocation
	mesh.vertices = calloc(num_vertex, sizeof(vertex_t));

	for (int i = shape.face_offset; i < shape.length; i += 3) {
		if (i < 20) {
			printf("Position: ");
			for (int j = 0; j < 3; j++) {
				printf("%f, ", attrib.vertices[attrib.faces[i + j].v_idx]);
			}
			printf("\t");

			printf("Texture: ");
			for (int j = 0; j < attrib.face_num_verts[i]; j++) {
				printf("%f, ", attrib.texcoords[attrib.faces[i].vt_idx + j]);
			}
			printf("\t");

			printf("Normal: ");
			for (int j = 0; j < attrib.face_num_verts[i]; j++) {
				printf("%f, ", attrib.normals[attrib.faces[i].vn_idx + j]);
			}
			printf("\n");
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

model_data_t model_load(const char *path)
{
	tinyobj_shape_t *shape = NULL;
	unsigned long num_shapes;

	tinyobj_material_t *material = NULL;
	unsigned long num_materials;
	
	tinyobj_attrib_t attrib;

	tinyobj_attrib_init(&attrib);

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
		return (model_data_t) { NULL, 0, NULL };
	}

	model_data_t loaded = {0};
	loaded.path = strdup(path);

	for (int i = 0; i < num_materials; i++) {
		printf("Skip\n");
	}

	for (int i = 0; i < num_shapes; i++) {
		(void) load_mesh(shape[i], attrib);
	}

	loaded.len_meshes = num_shapes;

	tinyobj_attrib_free(&attrib);

	if (shape)
		tinyobj_shapes_free(shape, num_shapes);

	if (material)
		tinyobj_materials_free(material, num_materials);

	return loaded;
}

