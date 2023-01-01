#pragma once

#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> Split(const std::string& text, const char delimiter = '\n');

void ReplaceAll(std::string& str, const std::string& from, const std::string& to);