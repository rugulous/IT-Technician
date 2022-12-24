#include "Game.h"

#include <GLFW/glfw3.h>

void Game::Init() {
	this->isRunning = true;
}

void Game::ProcessInput() {
	if (this->keys[GLFW_KEY_ESCAPE]) {
		this->isRunning = false;
	}
}