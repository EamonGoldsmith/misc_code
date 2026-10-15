#ifndef TEXTURE_H_
#define TEXTURE_H_

#define FOR_TEXTURE_TYPES(DO) \
	DO(DIFFUSE, ...) \
	DO(SPECULAR, ...) \
	DO(EMISSION, ...)

#define DEFINE_ENUM(type, ...) TEXTURE_##type,

enum texture_types {
	FOR_TEXTURE_TYPES(DEFINE_ENUM)
};

typedef struct
{
	int width, height, channels;
	unsigned int id;
	enum texture_types type;
} texture_t;

#define SWITCH_CASE(type, ...) case TEXTURE_##type: return #type;

static inline const char *texture_type_str(texture_t texture) {
	switch (texture.type) {
		FOR_TEXTURE_TYPES(SWITCH_CASE)
	}
	return "NOT FOUND";
}

int load_texture(const char *filename, texture_t *tex, int format,
	enum texture_types type);
void destroy_texture(texture_t *tex);

#endif // TEXTURE_H_
