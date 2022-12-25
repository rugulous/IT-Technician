#include "Game.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/ResourceManager.h"

#include "State/TestState.h"
#include "State/OverworldState.h"

Game::~Game() {
	if (_currentState != nullptr) {
		delete _currentState;
	}
}

void Game::Init() {
	ResourceManager::LoadShader("sprite", "Shader/Sprite/");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(600), static_cast<float>(800), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	ResourceManager::LoadTexture("face", "Resource/Texture/awesomeface.png", true);
	ResourceManager::LoadTexture("tile", "Resource/Texture/tile.png");

	_currentState = new TestState();

	this->isRunning = true;
}

void Game::ProcessInput() {
	if (this->keys[GLFW_KEY_ESCAPE]) {
		this->isRunning = false;
	}
}

void Game::Update(double dt) {
	if (_currentState != nullptr) {
		int res = _currentState->Update(dt);

		if (res == 1) {
			delete _currentState;
			_currentState = new OverworldState();
		}
	}
}

void Game::Render() {
	if (_currentState != nullptr) {
		_currentState->Render();
	}
}