#pragma once

#include <string>
#include <map>
#include <vector>

#include "UI/Shader.h"
#include "UI/Texture2D.h"


class ResourceManager {
public:
	static std::string ReadFile(const std::string& filePath);

	static Shader* LoadShader(const std::string& name, std::string filePath, const char* vertexPath = "vertex.sh", 
		const char* fragmentPath = "fragment.sh", const char* geometryPath = nullptr);
	static Shader* GetShader(const std::string&name);
	static void ReleaseShader(const std::string&name);

	static Texture2D LoadTexture(const std::string&name, const char* file, bool alpha = false);
	static Texture2D GetTexture(const std::string&name);
	static void ReleaseTexture(const std::string&name);

	static std::vector<std::vector<unsigned int>> LoadMap(const char* file);

	static void Clear();

private:
	static std::map<std::string, Shader> _shaders;
	static std::map<std::string, Texture2D> _textures;

	static std::string _JoinPath(const char* start, const char* end);
	static std::string _JoinPath(std::string start, const char* end);
};