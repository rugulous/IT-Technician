#pragma once

#include <array>

#include "Engine/UI/SpriteRenderer.h"
#include "State/IGameState.h"

class Game {
public:
	~Game();

	bool isRunning = false;
	std::array<bool, 1024> keys;

	void Init();
	void ProcessInput();
	void Update(double dt);
	void Render();

private:
	IGameState* _currentState;
};