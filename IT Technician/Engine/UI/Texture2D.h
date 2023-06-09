#pragma once

#include <glm/glm.hpp>

struct TextureFormat {
	unsigned int format;
	unsigned int imageFormat;
};

struct TextureWrap {
	unsigned int x;
	unsigned int y;
};

struct TextureFilter {
	unsigned int min;
	unsigned int max;
};

class Texture2D {
public:
	explicit Texture2D(bool alpha = false);

	void Load(unsigned int width, unsigned int height, unsigned const char* data);
	void Bind() const;
	void Clear();

	glm::vec4 GetDefaultCoords();
	void CalculateTextureCoords(glm::vec4* coords);

	unsigned int ID = 0;
private:
	
	unsigned int _width = 0;
	unsigned int _height = 0;

	TextureFormat _format;
	TextureWrap _wrap;
	TextureFilter _filter;
};