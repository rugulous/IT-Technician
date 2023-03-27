#pragma once

#include "Texture2D.h"

class PostProcessor {
public:
	PostProcessor(int width, int height);

	void BeginRender();
	void Render(float time);
	void EndRender();

private:
	unsigned int _MSFBO;
	unsigned int _FBO;
	unsigned int _RBO;
	unsigned int _VAO;

	Texture2D _texture;

	int _width;
	int _height;

	void _initRenderData();
};