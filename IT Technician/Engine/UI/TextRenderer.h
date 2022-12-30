#pragma once

#include <map>
#include <string>

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>

struct Character {
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class TextRenderer {
public:
	TextRenderer(unsigned int width, unsigned int height);

	bool Load(std::string fontPath, unsigned int size);
	void RenderText(std::string text, float x, float y, float scale, glm::vec3 colour = glm::vec3(1.0f));

private:
	std::map<char, Character> _characters;
	unsigned int _VAO;
	unsigned int _VBO;
};