#pragma once

#include "IGameState.h"

#include <string>
#include <array>
#include <vector>

#include "../Engine/UI/TextRenderer.h"

class HackerState : public IGameState {
public:
	~HackerState() override;

	void ProcessInput(std::array<bool, 1024>* keys) override;
	int Update(double dt) override;
	void Render() override;

private:
	std::vector<std::string> _fullText;
	int _progress = 0;

	std::vector<std::string> _buffer;

	std::array<bool, 1024> _processedKeys;

	TextRenderer* _renderer;

	void Init() override;
	void _addText(const std::string& text);

	//TODO: move to string helper or something
	std::vector<std::string> _splitText(const std::string& text);
	void _replaceAll(std::string& string, const std::string& from, const std::string& to) const;
};