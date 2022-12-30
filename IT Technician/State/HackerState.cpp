#include "HackerState.h"

#include <GLFW/glfw3.h>
#include <sstream>

#include "../Engine/ResourceManager.h"

HackerState::HackerState() {
	_Init();
}

HackerState::~HackerState() {
	Release();
	delete _renderer;
}

void HackerState::ProcessInput(std::array<bool, 1024>* keys) {

	for (int i = 0; i < 1024; i++) {
		if ((*keys)[i] && !_processedKeys[i]) {
			_progress++;

			if (_progress < _fullText.size()) {
				_addText(_fullText[_progress]);
			}

			_processedKeys[i] = true;
		}
		else if (!(*keys)[i] && _processedKeys[i]) {
			_processedKeys[i] = false;
		}
	}
}

int HackerState::Update(double dt) {
	if (_progress > _fullText.size()) {
		return 1; //TODO: return to last place
	}

	return 0;
}

void HackerState::Render() {
	for (int i = 0; i < _buffer.size(); i++) {
		if (_buffer[i].empty()) {
			continue;
		}

		_renderer->RenderText(_buffer[i], 20, 20 + (i * 52), 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void HackerState::_Init() {
	_renderer = new TextRenderer(792, 594);
	_renderer->Load("Resource/Font/inconsolata.ttf", 16);

	_fullText = _splitText(ResourceManager::ReadFile("Resource/String/hacker.txt"));
	_progress = 0;

	_addText("HacK3rMAn L337 k0N50l3");
}

std::vector<std::string> HackerState::_splitText(std::string text){
	std::vector<std::string> tokens;

	std::stringstream ss(text);
	std::string token;
	while (std::getline(ss, token, '\n')) {
		_replaceAll(token, "\t", "    ");
		tokens.push_back(token);
	}

	return tokens;
}

void HackerState::_replaceAll(std::string& str, const std::string& from, const std::string& to){
	if (from.empty()) {
		return;
	}

	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

void HackerState::_addText(std::string text) {
	if (_buffer.size() < 11) {
		_buffer.push_back(text);
		return;
	}

	for (int i = 0; i < _buffer.size() - 1; i++) {
		_buffer[i] = _buffer[i + 1];
	}

	_buffer[_buffer.size() - 1] = text;
}