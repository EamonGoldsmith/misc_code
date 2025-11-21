#ifndef WINDOW_H_
#define WINDOW_H_

#include "state.h"

int open_window(const char *title, const int width, const int height);
void close_window();

void poll_events(state_t *state, const int width, const int height);
void swap_buffers();
float window_time();

#endif // WINDOW_H_
