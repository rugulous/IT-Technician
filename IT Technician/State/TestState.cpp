#include "TestState.h"

#include "../Engine/ResourceManager.h"

TestState::TestState() {
	this->_Init();
}

TestState::~TestState() {
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
	_renderer->DrawSprite(face, glm::vec2(0.0f, 0.0f), glm::vec2(600, 800), _rotation);
}

void TestState::_Init() {
	_renderer = new SpriteRenderer();
}