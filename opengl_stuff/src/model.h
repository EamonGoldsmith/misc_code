#ifndef MODEL_H_
#define MODEL_H_

#include "mesh.h"

typedef struct
{
	mesh_t *meshes;
	size_t len_meshes;
	const char *path;
} model_t;

model_t model_load(const char *path, const char *bin_path); 
int model_setup(model_t *model);
void draw_model(model_t *model);
void destroy_model(model_t *model);

#endif // MODEL_H_
