#ifndef TEXTURE_H_
#define TEXTURE_H_

typedef struct
{
	int width, height, channels;
	unsigned int texture;
} texture_t;

int load_texture(const char *filename, texture_t *tex, int format);
void destroy_texture(texture_t *tex);

#endif // TEXTURE_H_
