#include "renderer.h"
#include "window.h"

#include <stdio.h>

float time = 0.0f;

int main(int argc, char *argv[])
{
	printf("starting...\n");

	// open a new window
	if (!open_window()) {
		return -1;
	}

	// start opengl renderer
	if (!start_renderer()) {
		return -1;
	}

	compile_shaders();
	setup_buffers();

	// game loop
	state_t state;
	while (state.running) {
		poll_events(&state);
		render(time);
		swap_buffers();
		time += 0.01f;
	}

	close_renderer();
	close_window();
	return 0;
}
