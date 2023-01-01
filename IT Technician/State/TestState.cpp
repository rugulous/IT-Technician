#include "TestState.h"

#include "../Engine/ResourceManager.h"

TestState::~TestState() {
	Release();
	delete _renderer;
}

StateOutcome TestState::Update(double dt) {
	_rotation += 0.1f;

	if (_rotation > 360) {
		return {
			LOAD_STATE,
			true,
			1
		};
	}

	return StateOutcome();
}

void TestState::Render() {
	Texture2D face = ResourceManager::GetTexture("face");
	_renderer->DrawSprite(face, glm::vec2(0.0f, 0.0f), glm::vec2(594, 792), _rotation);
}

void TestState::Release() {
	ResourceManager::ReleaseTexture("face");
}

void TestState::Init() {
	_renderer = new SpriteRenderer();
	ResourceManager::LoadTexture("face", "Resource/Texture/awesomeface.png", true);
}