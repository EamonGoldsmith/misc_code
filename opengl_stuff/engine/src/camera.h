#ifndef CAMERA_H_
#define CAMERA_H_

#include "state.h"
#include <cglm/cglm.h>

typedef struct
{
    float velocity;
    float sensitivity;

    float pitch, yaw;

    vec3 pos;
    vec3 front;
    vec3 up;
} camera_t;

void init_camera(camera_t *camera);
void camera_move(camera_t *camera, state_t *state);
void camera_look(camera_t *camera, float dx, float dy);

#endif // CAMERA_H_