#include "camera.h"

#include <SDL3/SDL_keycode.h>

void init_camera(camera_t *camera)
{
    camera->sensitivity = 0.001f;
    camera->velocity = 0.005f;

    glm_vec3_copy((vec3){1.0f, 0.7f, 4.2f}, camera->pos);
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->front);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->up);

    camera->pitch = 0.0f;
    camera->yaw = -90.f;
}

void camera_move(camera_t *camera, state_t *state)
{
	if (state->keys[SDL_SCANCODE_W]) {
		glm_vec3_muladds(camera->front, camera->velocity, camera->pos);
	}
	if (state->keys[SDL_SCANCODE_S]) {
		glm_vec3_mulsubs(camera->front, camera->velocity, camera->pos);
	}

	vec3 strafe_axis;
	glm_cross(camera->front, camera->up, strafe_axis);
	glm_normalize(strafe_axis);

	if (state->keys[SDL_SCANCODE_A]) {
		glm_vec3_mulsubs(strafe_axis, camera->velocity, camera->pos);
	}
	if (state->keys[SDL_SCANCODE_D]) {
		glm_vec3_muladds(strafe_axis, camera->velocity, camera->pos);
	}

	//printf("camera position (%.1f, %.1f, %.1f)\n",
	//	camera->pos[0], camera->pos[1], camera->pos[2]);
}

void camera_look(camera_t *camera, float dx, float dy)
{
    // looking
	float sensitivity = 0.1f;
	dx *= sensitivity;
	dy *= sensitivity;

	camera->yaw += dx;
	camera->pitch += dy;

    // don't break your neck
    camera->pitch = glm_clamp(camera->pitch, -89.0f, 89.0f);

    // convert pitch/yaw into vector
	vec3 direction;
	direction[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
	direction[1] = sin(glm_rad(camera->pitch));
	direction[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
	glm_normalize(direction);

	glm_vec3_copy(direction, camera->front);
}