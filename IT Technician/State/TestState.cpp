#include "TestState.h"

#include "../Engine/ResourceManager.h"

TestState::TestState() {
	this->_Init();
}

TestState::~TestState() {
	Release();
	delete _renderer;
}

int TestState::Update(double dt) {
	_rotation += 0.1;

	if (_rotation > 360) {
		return 1;
	}

	return 0;
}

void TestState::Render() {
	Texture2D face = ResourceManager::GetTexture("face");
	_renderer->DrawSprite(face, glm::vec2(0.0f, 0.0f), glm::vec2(594, 792), _rotation);
}

void TestState::Release() {
	ResourceManager::ReleaseTexture("face");
}

void TestState::_Init() {
	_renderer = new SpriteRenderer();
	ResourceManager::LoadTexture("face", "Resource/Texture/awesomeface.png", true);
}