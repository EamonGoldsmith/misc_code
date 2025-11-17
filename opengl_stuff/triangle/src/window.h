#ifndef WINDOW_H_
#define WINDOW_H_

typedef struct {
	int running;
} state_t;

int open_window();
void close_window();

void poll_events(state_t *state);
void swap_buffers();

#endif // WINDOW_H_
