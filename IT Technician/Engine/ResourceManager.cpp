#include "ResourceManager.h"

#include <fstream>
#include <sstream>

std::string ResourceManager::ReadFile(const char* filePath) {
	std::string data = "";

	std::ifstream stream(filePath);
	std::stringstream content;

	content << stream.rdbuf();

	stream.close();
	data = content.str();

	return data;
}