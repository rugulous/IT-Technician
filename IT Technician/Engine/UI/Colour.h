#pragma once

struct Colour {
public:
	Colour(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

	float red;
	float green;
	float blue;
	float alpha;
};