#pragma once

#include "IGameState.h"
#include "../Engine/UI/SpriteRenderer.h"
#include "../Engine/Entity/GameObject.h"

#include <vector>

class OverworldState : public IGameState {
public:
	OverworldState();
	~OverworldState() override;

	int Update(double dt) override;
	void Render() override;

private:
	void _Init() override;

	std::vector<GameObject> _tiles;
	SpriteRenderer* _renderer;
};