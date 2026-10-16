#ifndef MODEL_H_
#define MODEL_H_

#include "mesh.h"

typedef struct
{
	mesh_t **meshes;
	size_t len_meshes;
	const char *path;
} model_t;

model_t model_load(const char *path); 

#endif // MODEL_H_
