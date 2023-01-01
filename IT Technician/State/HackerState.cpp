#include "HackerState.h"

#include <GLFW/glfw3.h>
#include <sstream>

#include "../Engine/ResourceManager.h"
#include "../Engine/Util/String.h"

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

StateOutcome HackerState::Update(double dt) {
	if (_progress > _fullText.size()) {
		return {
			RESTORE
		};
	}

	return StateOutcome();
}

void HackerState::Render() {
	for (int i = 0; i < _buffer.size(); i++) {
		if (_buffer[i].empty()) {
			continue;
		}

		_renderer->RenderText(_buffer[i], 20, (float)(20 + (i * 52)), 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void HackerState::Init() {
	_renderer = new TextRenderer(792, 594);
	_renderer->Load("Resource/Font/inconsolata.ttf", 16);

	_fullText = Split(ResourceManager::ReadFile("Resource/String/hacker.txt"), '\n');
	_progress = 0;

	std::string startText("HacK3rMAn L337 k0N50l3");
	_addText(startText);
}

void HackerState::_addText(std::string& text) {
	ReplaceAll(text, "\t", "    ");

	if (_buffer.size() < 11) {
		_buffer.push_back(text);
		return;
	}

	for (int i = 0; i < _buffer.size() - 1; i++) {
		_buffer[i] = _buffer[i + 1];
	}

	_buffer[_buffer.size() - 1] = text;
}