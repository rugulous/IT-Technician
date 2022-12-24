#pragma once

#include <string>

class ResourceManager {
public:
	static std::string ReadFile(const char* filePath);
};