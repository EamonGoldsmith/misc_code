#include "renderer.h"
#include "window.h"
#include <SDL3/SDL_stdinc.h>

float time = 0.0f;
const char *title = "something?";
const int width = 1066, height = 800;

int main(int argc, char *argv[])
{
	// open a new window
	if (!open_window(title, width, height)) {
		return -1;
	}

	// start opengl renderer
	if (!start_renderer(width, height)) {
		return -1;
	}

	compile_shaders();
	setup_buffers();

	// game loop
	state_t state;
	while (state.running) {
		poll_events(&state, width, height);
		render(window_time(), &state);
		swap_buffers();
	}

	close_renderer();
	close_window();
	return 0;
}
