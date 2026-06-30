#include "window.h"

#include <stdbool.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

// these are globals because I'll only ever need one window??
Display *display;
Window window;
Window root;
GLXContext ctx;

typedef GLXContext (*glXCreateContextAttribsARBProc)
	(Display*, GLXFBConfig, GLXContext, Bool, const int*);

int open_window()
{
	// get current display
	display = XOpenDisplay(0);
	if (!display) {
		fprintf(stderr, "Cannot open X display\n");
		return -1;
	}

	// choose visual
	static int att[] = {
		GLX_RGBA,
		GLX_DEPTH_SIZE,
		24,
		GLX_DOUBLEBUFFER,
		None,
	};

	XVisualInfo *vi = glXChooseVisual(display, 0, att);
	if (!vi) {
		fprintf(stderr, "No appropriate visual found\n");
		return -1;
	}

	// get reference to root window
	root = DefaultRootWindow(display);

	int win_width = 800;
	int win_height = 600;

	Colormap cmap = XCreateColormap(display, root, vi->visual, AllocNone);

	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask;

	// create a new window
	window = XCreateWindow(display, root,
			10, 10, win_width, win_height,
			0, vi->depth, InputOutput,
			vi->visual, CWColormap | CWEventMask, &swa);

	// set parameters for opengl context
	static int visual_attribs[] = {
		GLX_X_RENDERABLE, true,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_DOUBLEBUFFER, true,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_DOUBLEBUFFER, true,
		GLX_STENCIL_SIZE, 8,
		None,
	};

	// create opengl context
	int num_fb = 0;
	GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display),
			visual_attribs, &num_fb);

	if (!fbc) {
		fprintf(stderr, "glXChooseFBConfig() failed\n");
		return -1;
	}

	// find context setting function
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB =
	(glXCreateContextAttribsARBProc)
	glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

	if (!glXCreateContextAttribsARB) {
		fprintf(stderr, "glXCreateContextAttribsARB() not found\n");
		return -1;
	}

	// set desired minimum OpenGL version
	static int context_attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
		GLX_CONTEXT_MINOR_VERSION_ARB, 2,
		None
	};

	// create modern opengl context
	ctx = glXCreateContextAttribsARB(display, fbc[0], NULL, true,
						context_attribs);

	if (!ctx) {
		printf("Failed to create OpenGL context. Exiting.\n");
		return -1;
	}

	XMapWindow(display, window);
	XStoreName(display, window, "BASIC WINDOW");
	glXMakeCurrent(display, window, ctx);

	// print version info
	int major = 0, minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("OpenGL context created.\nVersion %d.%d\nVendor %s\nRenderer %s\n",
		major, minor,
		glGetString(GL_VENDOR),
		glGetString(GL_RENDERER));

	// start glew
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "GLEW failed to start because %s\n",
		glewGetErrorString(err));
	}

	// grab the pointer
	XGrabPointer(display, window, True,
		PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
		GrabModeAsync, GrabModeAsync,
		None, None, CurrentTime);

	return 1;
}

void close_window()
{
	printf("Closing window\n");

	// destroy opengl context nicely
	glXMakeCurrent(display, None, NULL);
	glXDestroyContext(display, ctx);

	// close window
	XDestroyWindow(display, window);
	XCloseDisplay(display);
}

void set_key(int key, int press, state_t *state)
{
	switch (key)
	{
		case 9: state->running = 0; break;
		case 25: state->keys.w = press; break;
		case 38: state->keys.a = press; break;
		case 39: state->keys.s = press; break;
		case 40: state->keys.d = press; break;
		case 50: state->keys.shift = press; break;
		case 65: state->keys.space = press; break;
		default:
			// unhandled key
			break;
	}
}

void poll_events(state_t *state)
{
	unsigned int mask;
	int root_x, root_y;
	Window child;

	// update mouse pointer
	XQueryPointer(
		display, window, &root, &child,
        &root_x, &root_y, &state->mouse_x, &state->mouse_y, &mask);

	XWarpPointer(display, window, window,
		state->mouse_x, state->mouse_y,
		800, 600,
		400, 300);

	// poll events
	while (XPending(display)) {
		XEvent ev;
		XNextEvent(display, &ev);

		if (ev.type == KeyPress) {
			//printf("keycode: %i\n", ev.xkey.keycode);
			set_key(ev.xkey.keycode, 1, state);
		} else if (ev.type == KeyRelease) {
			set_key(ev.xkey.keycode, 0, state);
		}
	}
}

void swap_buffers()
{
	glXSwapBuffers(display, window);
}
