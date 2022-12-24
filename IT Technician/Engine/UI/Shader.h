#pragma once

class Shader {
public:
	~Shader();

	bool Compile(const char* vertexDefinition, const char* fragmentDefinition, const char* geometryDefinition = "");

private:
	int _ID = -1;

	unsigned int _BuildShader(unsigned int type, const char* definition) const;
};