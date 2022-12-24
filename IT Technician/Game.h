#pragma once

#include <array>

#include "Engine/UI/SpriteRenderer.h"

class Game {
public:
	~Game();

	bool isRunning = false;
	std::array<bool, 1024> keys;

	void Init();
	void ProcessInput();
	void Update();
	void Render();

private:
	SpriteRenderer* _renderer;
};