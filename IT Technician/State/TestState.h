#pragma once

#include "IGameState.h"
#include "../Engine/UI/SpriteRenderer.h"

class TestState : public IGameState {
public:
	~TestState() override;

	void Init() override;
	StateOutcome Update(double dt) override;
	void Render() override;
	void Release() override;

private:
	SpriteRenderer* _renderer;
	float _rotation;

};