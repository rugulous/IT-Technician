#include "ResourceManager.h"

#include <fstream>
#include <sstream>

std::map<std::string, Shader> ResourceManager::_shaders;

std::string ResourceManager::ReadFile(const char* filePath) {
	std::string data = "";

	std::ifstream stream(filePath);
	std::stringstream content;

	content << stream.rdbuf();

	stream.close();
	data = content.str();

	return data;
}

Shader* ResourceManager::LoadShader(std::string name, const char* filePath, const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	char end = filePath[strlen(filePath) - 1];
	if (end != '\\' && end != '/') {
		filePath += '/';
	}

	std::string vertexSource = ResourceManager::_JoinPath(filePath, vertexPath);
	std::string vertex = ResourceManager::ReadFile(vertexSource.c_str());

	std::string fragmentSource = ResourceManager::_JoinPath(filePath, fragmentPath);
	std::string fragment = ResourceManager::ReadFile(fragmentSource.c_str());

	std::string geometry = "";
	if (geometryPath != "" && geometryPath != nullptr) {
		std::string geometrySource = ResourceManager::_JoinPath(filePath, geometryPath);
		geometry = ResourceManager::ReadFile(geometrySource.c_str());
	}

	Shader shader;
	if (!shader.Compile(vertex.c_str(), fragment.c_str(), geometry.c_str())) {
		return nullptr;
	}

	_shaders[name] = shader;
	return &_shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return _shaders[name];
}

void ResourceManager::Clear() {
	_shaders.clear();
}

std::string ResourceManager::_JoinPath(const char* start, const char* end) {
	std::string path(start);
	path.append(end);

	return path;
}

