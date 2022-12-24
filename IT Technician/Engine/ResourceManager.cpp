#include "ResourceManager.h"

#include <fstream>
#include <sstream>
#include "../stb_image.h"

std::map<std::string, Shader> ResourceManager::_shaders;
std::map<std::string, Texture2D> ResourceManager::_textures;

std::string ResourceManager::ReadFile(const char* filePath) {
	std::string data = "";

	std::ifstream stream(filePath);
	std::stringstream content;

	content << stream.rdbuf();

	stream.close();
	data = content.str();

	return data;
}

Shader ResourceManager::LoadShader(std::string name, const char* filePath, const char* vertexPath, const char* fragmentPath, const char* geometryPath)
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
	shader.Compile(vertex.c_str(), fragment.c_str(), geometry.c_str());
	
	_shaders[name] = shader;
	return shader;
}

Shader ResourceManager::GetShader(std::string name)
{
	return _shaders[name];
}

Texture2D ResourceManager::LoadTexture(std::string name, const char* file, bool alpha)
{
	Texture2D texture(alpha);
	int width;
	int height;
	int channels;

	unsigned char* data = stbi_load(file, &width, &height, &channels, 0);
	texture.Load(width, height, data);
	stbi_image_free(data);

	_textures[name] = texture;
	return texture;
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	return _textures[name];
}

void ResourceManager::Clear() {
	for (auto iter : _shaders) {
		iter.second.Clear();
	}

	_shaders.clear();

	for (auto iter : _textures) {
		iter.second.Clear();
	}
	
	_textures.clear();
}

std::string ResourceManager::_JoinPath(const char* start, const char* end) {
	std::string path(start);
	path.append(end);

	return path;
}

