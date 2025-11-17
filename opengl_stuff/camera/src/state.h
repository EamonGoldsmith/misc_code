
#ifndef STATE_H_
#define STATE_H_

struct key_codes {
    int w;
    int a;
    int s;
    int d;
    int shift;
    int space;
    int esc;
};

// global flags/vars that anything can change at any time
typedef struct {
    struct key_codes keys;
    int mouse_x, mouse_y;
	int running;
} state_t;

#endif // STATE_H_