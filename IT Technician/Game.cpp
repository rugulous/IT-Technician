#include "Game.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/ResourceManager.h"

Game::~Game() {
	delete _renderer;
}

void Game::Init() {
	ResourceManager::LoadShader("sprite", "Shader/Sprite/");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(600), static_cast<float>(800), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	ResourceManager::LoadTexture("face", "Resource/Texture/awesomeface.png", true);

	_renderer = new SpriteRenderer();

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
	Texture2D face = ResourceManager::GetTexture("face");
	_renderer->DrawSprite(face, glm::vec2(0.0f, 0.0f), glm::vec2(600, 800), 0.0f);
}