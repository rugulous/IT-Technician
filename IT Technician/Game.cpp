#include "Game.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include "Engine/ResourceManager.h"

void Game::Init() {
	if (ResourceManager::LoadShader("sprite", "Shader/Sprite/") == nullptr) {
		std::cout << "Failed to load Sprite Shader" << std::endl;
	}

	this->isRunning = true;
}

void Game::ProcessInput() {
	if (this->keys[GLFW_KEY_ESCAPE]) {
		this->isRunning = false;
	}
}