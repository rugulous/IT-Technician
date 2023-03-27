#pragma once

#include <array>

#include "Engine/UI/SpriteRenderer.h"
#include "State/IGameState.h"
#include "Engine/UI/PostProcessor.h"

class Game {
public:
	Game(int width, int height);
	~Game();

	bool isRunning = false;
	std::array<bool, 1024> keys;

	void Init();
	void ProcessInput();
	void Update(double dt);
	void Render();

private:
	int _width;
	int _height;

	IGameState* _currentState = nullptr;
	IGameState* _previousState = nullptr;

	PostProcessor* _processor;

	void _changeState(IGameState *newState, bool destroy = false);
	void _restoreState(bool destroy = true);
};