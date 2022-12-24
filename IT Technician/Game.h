#pragma once

#include <array>

class Game {
public:
	bool isRunning = false;
	std::array<bool, 1024> keys;

	void Init();
	void ProcessInput();
	void Update();
	void Render();
};