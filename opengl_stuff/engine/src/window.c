#include "window.h"

#include <SDL3/SDL.h>
#include <GL/glew.h>

#include <stdbool.h>
#include <stdio.h>

#define ERR_PRINT(msg) SDL_LogError(SDL_LOG_CATEGORY_ERROR, msg ": %s\n", SDL_GetError());

SDL_Window *window;
SDL_Surface *surface;
SDL_GLContext ctx;

void print_available_renderers();

int open_window(const char *title, const int width, const int height)
{
	// init SDL
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		ERR_PRINT("Failed to init SDL");
		return -1;
	}

	SDL_Log("Starting...\n");

	// create a new window
	window = SDL_CreateWindow(title, width, height, 
		SDL_WINDOW_OPENGL |
		SDL_WINDOW_MOUSE_CAPTURE
	);

	if (window == NULL) {
		ERR_PRINT("Failed to create new window");
		return -1;
	}

	// get window surface
	surface = SDL_GetWindowSurface(window);
	
	if (surface == NULL) {
		ERR_PRINT("Failed to load window surface");
		return -1;
	}

	// set opengl context attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// create opengl context
	ctx = SDL_GL_CreateContext(window);
	
	if (ctx == NULL) {
		ERR_PRINT("Failed to create opengl context");
		return -1;
	}

	// trap cursor
	SDL_SetWindowRelativeMouseMode(window, true);

	// load glew
	glewExperimental = GL_TRUE; // required for modern core profiles
	GLenum err = glewInit();

	if (err != GLEW_OK) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"Failed to initialise glew: %s\n",
			(const char*)glewGetErrorString(err));
		return -1;
	}

	// print version info
	int major = 0, minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	SDL_Log("OpenGL context created.\nVersion %d.%d\nVendor %s\nRenderer %s\n",
		major, minor,
		glGetString(GL_VENDOR),
		glGetString(GL_RENDERER));

	return 1;
}

void close_window()
{
	SDL_Log("Closing window\n");

	// destroy opengl context nicely
	SDL_GL_DestroyContext(ctx);

	// close window
	SDL_DestroySurface(surface);
	SDL_DestroyWindow(window);
	surface = NULL;
	window = NULL;

	SDL_Quit();
}

void poll_events(state_t *state, const int width, const int height)
{
	// request latest events
	SDL_PumpEvents();

	// reset mouse
	SDL_WarpMouseInWindow(window, (float)width / 2, (float)height / 2);
	state->mouse_x = 0;
	state->mouse_y = 0;

	// update keys
	state->keys = (bool*)SDL_GetKeyboardState(NULL);

	// poll events
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_EVENT_QUIT:
			state->running = false;
			break;
		case SDL_EVENT_MOUSE_MOTION:
			state->mouse_x = ev.motion.x - ((float)width / 2);
			state->mouse_y = ((float)height / 2) - ev.motion.y;
			break;
		}
	}
}

void swap_buffers()
{
	SDL_GL_SwapWindow(window);
}

float window_time()
{
	return (float)SDL_GetTicks();
}