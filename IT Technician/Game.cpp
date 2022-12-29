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

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(594), static_cast<float>(792), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	_currentState = new TestState();

	this->isRunning = true;
}

void Game::ProcessInput() {
	if (this->keys[GLFW_KEY_ESCAPE]) {
		this->isRunning = false;
	}

	if (_currentState != nullptr) {
		_currentState->ProcessInput(&this->keys);
	}
}

void Game::Update(double dt) {
	if (_currentState != nullptr) {
		int res = _currentState->Update(dt);

		if (res == 1) {
			_changeState(new OverworldState());
		}
		else if (res == 2) {
			_changeState(nullptr); //TODO add minigame
		}
	}
}

void Game::Render() {
	if (_currentState != nullptr) {
		glClearColor(_currentState->background.red, _currentState->background.green, _currentState->background.blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		_currentState->Render();
		return;
	}

	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Game::_changeState(IGameState* newState){
	if (_currentState != nullptr) {
		_currentState->Release();
		delete _currentState;
	}

	_currentState = newState;
}
