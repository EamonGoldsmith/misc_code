#include "text.h"

#include <GL/glew.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_FONT_PATH "/usr/share/fonts/gnu-free/FreeMono.otf"
#define NUM_CHARS (95)

struct character 
{
	unsigned int texture_id;
	ivec2d size;
	ivec2d bearing;
	unsigned int advance;
};

FT_Face face;
FT_Library ft;
struct character char_map[NUM_CHARS];

int load_font_face()
{
	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
		return -1;
	}

	if (FT_New_Face(ft, DEFAULT_FONT_PATH, 0, &face)) {
		fprintf(stderr, "Failed to load font %s\n", DEFAULT_FONT_PATH);
		return -1;
	}
	FT_Set_Pixel_Sizes(face, 0, 48);

	if (!load_textures()) {
		fprintf(stderr, "Failed to create character map\n");
		return -1;
	}

	return 1;
}

struct character load_character(char c)
{
	// load character glyph, starting from space
	if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
		fprintf(stderr, "Failed to load glyph of %c\n", c+32);
	}

	// generate texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		face->glyph->bitmap.width,
		face->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer
	);

	// set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// store for later use
	struct character result = {
		texture,
		{ face->glyph->bitmap.width, face->glyph->bitmap.rows },
		{ face->glyph->bitmap_left, face->glyph->bitmap_top },
		face->glyph->advance.x
	};
	
	return result;
}

int load_textures()
{
	for (unsigned int c = 0; c < NUM_CHARS; c++) {
		char_map[c] = load_character(c + 32);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// all glyph textures are loaded, so we don't need this anymore
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return 1;
}

void render_character(
	unsigned int shader,
	char c,
	fvec3d pos, // z component is used for scale
	fvec3d color,
	GLuint VAO, GLuint VBO)
{
	glUseProgram(shader);

	// set shader params
	glUniform3f(
		glGetUniformLocation(shader, "textColor"),
		color.x, color.y, color.z
	);

	glActiveTexture(GL_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);

	struct character ch = char_map[c - 32];

	// update VBO with x, y, u, v
	float vertices[6][4] = {
		{ -0.5,		0.5,		0.0f, 1.0f },            
		{ -0.5,		-0.5,       0.0f, 0.0f },
		{ 0.5,		-0.5,       1.0f, 0.0f },

		{ 0.5,		-0.5,   0.0f, 0.0f },
		{ 0.5,		0.5,       1.0f, 1.0f },
		{ -0.5,		-0.5,   1.0f, 0.0f }           
	};

	glBindTexture(GL_TEXTURE_2D, ch.texture_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void render_string(
	unsigned int shader,
	char *str,
	fvec3d pos,
	fvec3d color,
	GLuint VAO, GLuint VBO)
{
	glUseProgram(shader);

	// set shader params
	glUniform3f(
		glGetUniformLocation(shader, "textColor"),
		color.x, color.y, color.z
	);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);	

	// iterate through all characters
	for (int i = 0; i < strlen(str); i++)
	{
		char c = str[i];
		struct character ch = char_map[c - 32];

		float scale = pos.z;
		float xpos = pos.x + ch.bearing.x * scale;
		float ypos = pos.y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		// update VBO
		float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },            
            { xpos,     ypos,       0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };

		// get into the weeds
		glBindTexture(GL_TEXTURE_2D, ch.texture_id);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);

		pos.x += (ch.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
