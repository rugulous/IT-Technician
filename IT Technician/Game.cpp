#include "Game.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/ResourceManager.h"

#include "State/TestState.h"
#include "State/OverworldState.h"
#include "State/HackerState.h"

Game::~Game() {
	if (_currentState != nullptr) {
		delete _currentState;
	}
}

void Game::Init() {
	ResourceManager::LoadShader("sprite", "Shader/Sprite/");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(594), static_cast<float>(792), 0.0f, -1.0f, 1.0f);
	Shader* sprite = ResourceManager::GetShader("sprite");
	sprite->Use();
	sprite->SetInteger("sprite", 0);
	sprite->SetMatrix4("projection", projection);

	_currentState = new TestState();
	_currentState->Init();

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
		//TODO: more meaningful return values
		int res = _currentState->Update(dt);

		if (res == 1) {
			_changeState(new OverworldState(), true);
		}
		else if (res == 2) {
			_changeState(new HackerState());
		}
		else if (res == 3) {
			_restoreState();
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

void Game::_changeState(IGameState* newState, bool destroy){
	if (_currentState != nullptr) {
		if (destroy) {
			_currentState->Release();
			delete _currentState;
		}
		else {
			_previousState = _currentState;
		}
	}

	if (newState != nullptr) {
		newState->Init();
	}

	_currentState = newState;
}

void Game::_restoreState(bool destroy) {
	if (_previousState == nullptr) {
		return;
	}

	IGameState* tmp = _currentState;
	_currentState = _previousState;

	if (destroy && tmp != nullptr) {
		tmp->Release();
		delete tmp;
		_previousState = nullptr;
	}
	else {
		_previousState = tmp;
	}
}