#include "TextRenderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../ResourceManager.h"

TextRenderer::TextRenderer(unsigned int width, unsigned int height) {
	Shader* shader = ResourceManager::LoadShader("text", "Shader/Text");
	shader->Use();
	shader->SetMatrix4("projection", glm::ortho(0.0f, (float)width, (float)height, 0.0f));
	shader->SetInteger("text", 0);

	glGenVertexArrays(1, &this->_VAO);
	glGenBuffers(1, &this->_VBO);
	glBindVertexArray(this->_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

bool TextRenderer::Load(std::string fontPath, unsigned int size) {
	this->_characters.clear();

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		//0 is success code
		return false;
	}

	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, size);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (int c = 32; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		_characters.insert(std::pair<char, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 colour) {
	Shader* shader = ResourceManager::GetShader("text");
	shader->Use();
	shader->SetVector3f("textColour", colour);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->_VAO);

	for (int i = 0; i < text.length(); i++) {
		Character c = _characters[text[i]];

		float xPos = x + c.bearing.x * scale;
		float yPos = y + (_characters['H'].bearing.y - c.bearing.y) * scale;
		float width = c.size.x * scale;
		float height = c.size.y * scale;

		float vertices[6][4] = {
			{ xPos, yPos + height, 0.0f, 1.0f },
			{ xPos + width, yPos, 1.0f, 0.0f },
			{ xPos, yPos, 0.0f, 0.0f },

			{ xPos, yPos + height, 0.0f, 1.0f },
			{ xPos + width, yPos + height, 1.0f, 1.0f },
			{ xPos + width, yPos, 1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, c.textureID);
		glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (c.advance >> 6) * scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
