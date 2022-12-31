#pragma once

#include <glm/glm.hpp>

#include "Texture2D.h"

class SpriteRenderer {
public:
	SpriteRenderer();
	~SpriteRenderer();

	void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f), glm::vec4* texturePos = nullptr);

private:
	unsigned int _quadVAO;
	unsigned int _VBO;

	void _InitRenderData();
};