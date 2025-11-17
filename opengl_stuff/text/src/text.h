#ifndef TEXT_H_
#define TEXT_H_

#include "vec.h"

// utilities for loading font and rendering .ttf
// requires include flag "-I/usr/include/freetype2"

#include <ft2build.h>
#include <freetype/freetype.h>

#include <GL/glew.h>

int load_font_face();

struct character load_character(char c);
int load_textures();

void render_character(
	unsigned int shader,
	char c,
	fvec3d pos, // z component is used for scale
	fvec3d color,
	GLuint VAO, GLuint VBO);

void render_string(
	unsigned int shader,
	char *str,
	fvec3d pos, // z component is used for scale
	fvec3d color,
	GLuint VAO, GLuint VBO);

#endif // TEXT_H_
