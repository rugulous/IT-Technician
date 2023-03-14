#pragma once

#include "IGameState.h"

#include <vector>

#include "../Engine/UI/SpriteRenderer.h"

struct Pipe {
	bool openTop;
	bool openBottom;
	bool openLeft;
	bool openRight;
};

class PipeState : public IGameState {
public:
	void Init() override;
	void ProcessInput(std::array<bool, 1024>* keys);
	StateOutcome Update(double dt) override;
	void Render() override;
	void Release() override;

private:
	SpriteRenderer* _renderer;

	std::vector<std::vector<std::pair<bool, int>>> _pipes;
	std::vector<Texture2D> _textures;
	std::vector<Pipe> _pipeConfig;

	const int _startX = 0;
	const int _startY = 0;

	const int _endX = 4;
	const int _endY = 4;

	int _selectedX = 0;
	int _selectedY = 0;

	float _rotation = 0;

	double _inputDelay = 0;
	bool _rotating = false;

	bool _solved = false;
	bool _done = false;

	bool _checkForPath();
};