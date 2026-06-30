#include "renderer.h"
#include "window.h"

#include <stdio.h>

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
	state.running = 1;
	while (state.running) {
		poll_events(&state);
		render();
		swap_buffers();
	}

	close_renderer();
	close_window();
	return 0;
}
