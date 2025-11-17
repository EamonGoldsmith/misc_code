#ifndef RENDERER_H_
#define RENDERER_H_

#include "state.h"

int start_renderer();
void compile_shaders();
void setup_buffers();
void render(float time, state_t *state);
void close_renderer();

#endif // RENDERER_H_
