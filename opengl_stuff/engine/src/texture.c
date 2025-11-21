
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>

int load_texture(const char *filename, texture_t *tex, int format)
{
	printf("Reading texture: %s\n", filename);
	glGenTextures(1, &tex->texture);
	printf("Attached texture to: %i\n", tex->texture);
	glBindTexture(GL_TEXTURE_2D, tex->texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char *data = NULL;
	data = stbi_load(filename,
		&tex->width, &tex->height, &tex->channels, 0);

	if (!data) {
		fprintf(stderr, "Failed to load texture: %s\n", filename);
		return -1;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, 
		tex->width, tex->height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return 1;
}

void destroy_texture(texture_t *tex)
{
	glDeleteTextures(0, &tex->texture);
}