
#ifndef STATE_H_
#define STATE_H_

#include <SDL3/SDL_keycode.h>

// global flags/vars that anything can change at any time
typedef struct {
    bool *keys;
    float mouse_x, mouse_y;
	bool running;
} state_t;

#endif // STATE_H_