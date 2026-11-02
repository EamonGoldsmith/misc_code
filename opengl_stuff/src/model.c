#include "model.h"
#include "model_loader.h"
#include "mesh.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void model_load_primitive(mesh_t *mesh, cgltf_primitive *prim)
{
	assert(prim != NULL);

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

    mesh->len_vertices = vertex_count;
    mesh->vertices = calloc(vertex_count, sizeof(vertex_t));
    assert(mesh->vertices != NULL);

    for (size_t i = 0; i < vertex_count; i++) {
        vertex_t *v = &mesh->vertices[i];

        cgltf_accessor_read_float(pos_accessor, i, v->position, 3);

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

	assert(prim->indices != NULL);

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

void parse_node(cgltf_node *node)
{
	printf("parsing node: %s\n", node->name);
	printf("this node has %li children\n", node->children_count);

	if (node->children_count > 0)
	{
		printf("->");
		parse_node(node->children[0]);
	}
}

void parse_scene(cgltf_scene *scene)
{
	printf("Model parsing scene: %s\n", scene->name);

	for (size_t i = 0; i < scene->nodes_count; i++)
	{
		parse_node(scene->nodes[i]);
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
	printf("Model has %li meshes\n", data->meshes_count);

	result = cgltf_load_buffers(&options, data, bin_path);
    assert(result == cgltf_result_success);

	model_t model = {0};
	model.len_meshes = data->meshes_count;
	model.meshes = calloc(model.len_meshes, sizeof(mesh_t));
	assert(model.meshes != NULL);

	parse_scene(data->scene);

    cgltf_free(data);
	return model;
}

void model_setup(model_t *model)
{
	assert(model != NULL);

	for (size_t i = 0; i < model->len_meshes; i++) 
	{
		mesh_setup(model->meshes + i);
	}
}

void model_draw(model_t *model, unsigned int shader)
{
	for (size_t i = 0; i < model->len_meshes; i++) 
	{
		mesh_draw(&model->meshes[i], shader);
	}
}

void destroy_model(model_t *model)
{
	assert(model != NULL);

	for (size_t i = 0; i < model->len_meshes; i++) 
	{
		mesh_destroy(&model->meshes[i]);
	}
}
