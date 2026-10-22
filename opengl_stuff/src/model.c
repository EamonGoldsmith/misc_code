#include "model.h"
#include "model_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void model_load_primitive(cgtlf_primitive *prim)
{

    cgltf_accessor *pos_accessor = NULL;
    cgltf_accessor *norm_accessor = NULL;
    cgltf_accessor *uv_accessor = NULL;

    for (size_t i = 0; i < prim->attributes_count; i++)
	{
        if (prim->attributes[i].type == cgltf_attribute_type_position)
            pos_accessor = prim->attributes[i].data;

        if (prim->attributes[i].type == cgltf_attribute_type_normal)
            norm_accessor = prim->attributes[i].data;

        if (prim->attributes[i].type == cgltf_attribute_type_texcoord)
            if (uv_accessor == NULL)
                uv_accessor = prim->attributes[i].data;
    }

    assert(pos_accessor != NULL);
    size_t vertex_count = pos_accessor->count;

	model_t loaded = {0};
	loaded.meshes = calloc(1, sizeof(mesh_t));
	mesh_t *mesh = &loaded.meshes[0];

    mesh->len_vertices = vertex_count;
    mesh->vertices = calloc(vertex_count, sizeof(vertex_t));
    assert(mesh->vertices != NULL);

    for (size_t i = 0; i < vertex_count; i++) {
        vertex_t *v = &mesh->vertices[i];

        cgltf_accessor_read_float(pos_accessor, i, v->position, 3);
		printf("loading vertex %li, (%f, %f, %f)\n", i,
				v->position[0],
				v->position[1],
				v->position[2]);

        if (norm_accessor) {
            cgltf_accessor_read_float(norm_accessor, i, v->normal, 3);
        }

        if (uv_accessor) {
            float uv[2];
            cgltf_accessor_read_float(uv_accessor, i, uv, 2);
            v->tex_coords[0] = uv[0];
            v->tex_coords[1] = uv[1];
            v->tex_coords[2] = 0.0f;
        }
    }

    if (prim->indices != NULL) {
        cgltf_accessor *index_accessor = prim->indices;
        size_t index_count = index_accessor->count;

        mesh->len_indices = index_count;
        mesh->indices = malloc(index_count * sizeof(unsigned int));
        assert(mesh->indices != NULL);

        for (size_t i = 0; i < index_count; i++) {
            mesh->indices[i] = 
				(unsigned int)cgltf_accessor_read_index(index_accessor, i);
        }
    }
}

model_t model_load(const char *path, const char *bin_path)
{
	assert(path != NULL);
	assert(strlen(path) > 0);

	printf("Loading model: %s\n", path);

    cgltf_options options = {0};
    cgltf_data *data = NULL;
	cgltf_result result = cgltf_parse_file(&options, path, &data);
    assert(result == cgltf_result_success);

	printf("Model has %li materials\n", data->materials_count);
	printf("Model has %li buffers\n", data->buffers_count);
	printf("Model has %li images\n", data->images_count);

	result = cgltf_load_buffers(&options, data, bin_path);
    assert(result == cgltf_result_success);

	printf("Model size %li\n", data->bin_size);

	for (size_t i = 0; i < data->meshes_count; i++)
	{
		cgltf_primitive *prim = &data->meshes[0].primitives[0];
	}

    cgltf_free(data);

	return (model_t){ NULL, 0, NULL };
}

int model_setup(model_t *model)
{
	return 1;
}

void model_draw(model_t *model)
{
	
}

void destroy_model(model_t *model)
{
	assert(model != NULL);
}
