#include "Texture2D.h"
#include <glad/glad.h>

Texture2D::Texture2D(bool alpha) {
	int format = alpha ? GL_RGBA : GL_RGB;

	this->_format.format = format;
	this->_format.imageFormat = format;

	this->_wrap.x = GL_REPEAT;
	this->_wrap.y = GL_REPEAT;

	this->_filter.min = GL_LINEAR;
	this->_filter.max = GL_LINEAR;

	glGenTextures(1, &this->ID);
}

void Texture2D::Load(unsigned int width, unsigned int height, unsigned const char* data) {
	_width = width;
	_height = height;

	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->_format.format, _width, _height, 0, this->_format.imageFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->_wrap.x);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->_wrap.y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->_filter.min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->_filter.max);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

void Texture2D::Clear() {
	glDeleteTextures(1, &this->ID);
}

glm::vec4 Texture2D::GetDefaultCoords() {
	return glm::vec4(0, this->_width, 0, this->_height);
}

void Texture2D::CalculateTextureCoords(glm::vec4* coords)
{
	coords->x = coords->x / this->_width;
	coords->y = coords->y / this->_width;
	coords->z = coords->z / this->_height;
	coords->w = coords->w / this->_height;
}
