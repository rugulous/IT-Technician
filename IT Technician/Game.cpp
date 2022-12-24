#include "Game.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include "Engine/ResourceManager.h"

void Game::Init() {
	ResourceManager::LoadShader("sprite", "Shader/Sprite/");

	ResourceManager::LoadTexture("face", "Resource/Texture/awesomeface.png", true);

	this->isRunning = true;
}

void Game::ProcessInput() {
	if (this->keys[GLFW_KEY_ESCAPE]) {
		this->isRunning = false;
	}
}

void Game::Update() {

}

void Game::Render() {

}