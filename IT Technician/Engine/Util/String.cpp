#include "String.h"

std::vector<std::string> Split(const std::string& text, const char delimiter) {
	std::vector<std::string> tokens;

	std::stringstream ss(text);
	std::string token;
	while (std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

void ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty()) {
		return;
	}

	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}