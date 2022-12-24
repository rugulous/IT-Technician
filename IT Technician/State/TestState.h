#pragma once

#include "IGameState.h"
#include "../Engine/UI/SpriteRenderer.h"

class TestState : public IGameState {
public:
	TestState();
	~TestState();

	void Init();
	void Update(double dt);
	void Render();

private:
	SpriteRenderer* _renderer;
	float _rotation;
};