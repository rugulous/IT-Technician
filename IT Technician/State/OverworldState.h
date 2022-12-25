#pragma once

#include "IGameState.h"
#include "../Engine/UI/SpriteRenderer.h"
#include "../Engine/Entity/GameObject.h"

#include <vector>

struct Size {
	float width;
	float height;
};

class OverworldState : public IGameState {
public:
	OverworldState();
	~OverworldState() override;

	void ProcessInput(std::array<bool, 1024> keys) override;
	int Update(double dt) override;
	void Render() override;

private:
	void _Init() override;

	std::vector<GameObject> _tiles;

	GameObject* _player;
	SpriteRenderer* _renderer;

	Size _tileSize;
	Size _mapSize;

	float _x = 0;
	float _y = 0;
};