#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

bool Shader::Compile(const char* vertexDefinition, const char* fragmentDefinition, const char* geometryDefinition)
{
	int vertex = this->_BuildShader(GL_VERTEX_SHADER, vertexDefinition);
	int fragment = this->_BuildShader(GL_FRAGMENT_SHADER, fragmentDefinition);
	int geometry = this->_BuildShader(GL_GEOMETRY_SHADER, geometryDefinition);

	this->_ID = glCreateProgram();
	glAttachShader(this->_ID, vertex);
	glAttachShader(this->_ID, fragment);

	if (geometry > 0) {
		glAttachShader(this->_ID, geometry);
	}

	glLinkProgram(this->_ID);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	if (geometry > 0) {
		glDeleteShader(geometry);
	}

	int success;
	glGetProgramiv(this->_ID, GL_LINK_STATUS, &success);

	return success;
}

Shader& Shader::Use() {
	glUseProgram(this->_ID);
	return *this;
}

void Shader::SetInteger(const char* name, const int value) const{
	glUniform1i(glGetUniformLocation(this->_ID, name), value);
}

void Shader::SetVector3f(const char* name, const glm::vec3& value) const{
	glUniform3f(glGetUniformLocation(this->_ID, name), value.x, value.y, value.z);
}

void Shader::SetMatrix4(const char* name, const glm::mat4& matrix) const{
	glUniformMatrix4fv(glGetUniformLocation(this->_ID, name), 1, false, glm::value_ptr(matrix));
}

void Shader::Clear() {
	if (this->_ID != -1) {
		glDeleteProgram(this->_ID);
	}
}

unsigned int Shader::_BuildShader(unsigned int type, const char* definition) const
{
	if (definition == nullptr || strlen(definition) == 0) {
		return 0;
	}

	int shader = glCreateShader(type);
	glShaderSource(shader, 1, &definition, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		return 0;
	}

	return shader;
}
