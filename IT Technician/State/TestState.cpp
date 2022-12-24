#include "TestState.h"

#include "../Engine/ResourceManager.h"

TestState::TestState() {
	this->Init();
}

TestState::~TestState() {
	delete _renderer;
}

void TestState::Init() {
	_renderer = new SpriteRenderer();
}

void TestState::Update(double dt) {
	_rotation += 0.1;

	if (_rotation > 360) {
		_rotation = 0;
	}
}

void TestState::Render() {
	Texture2D face = ResourceManager::GetTexture("face");
	_renderer->DrawSprite(face, glm::vec2(0.0f, 0.0f), glm::vec2(600, 800), _rotation);
}