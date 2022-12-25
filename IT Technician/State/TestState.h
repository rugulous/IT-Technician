#pragma once

#include "IGameState.h"
#include "../Engine/UI/SpriteRenderer.h"

class TestState : public IGameState {
public:
	TestState();
	~TestState() override;

	int Update(double dt) override;
	void Render() override;
	void Release() override;

private:
	SpriteRenderer* _renderer;
	float _rotation;

	void _Init() override;
};