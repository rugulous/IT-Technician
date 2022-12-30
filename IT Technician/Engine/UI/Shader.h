#pragma once

#include <glm/glm.hpp>

class Shader {
public:
	bool Compile(const char* vertexDefinition, const char* fragmentDefinition, const char* geometryDefinition = "");
	Shader& Use();

	void SetInteger(const char* name, const int value) const;
	void SetVector3f(const char* name, const glm::vec3& value) const;
	void SetMatrix4(const char* name, const glm::mat4& matrix) const;

	void Clear();

private:
	int _ID = -1;

	unsigned int _BuildShader(unsigned int type, const char* definition) const;
};