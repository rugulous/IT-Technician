#pragma once

#include "IGameState.h"
#include "../Engine/UI/SpriteRenderer.h"
#include "../Engine/Entity/GameObject.h"

#include <vector>

struct Size {
	float width;
	float height;
};

struct Tile {
	bool isSolid;
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

	std::vector<std::vector<Tile>> _tiles;

	GameObject* _player;
	SpriteRenderer* _renderer;

	glm::vec2 _tileSize;
	Size _mapSize;

	float _x = 0;
	float _y = 0;

	float _playerX = 4;
	float _playerY = 4;

	bool _canMove(int xOffset = 0, int yOffset = 0);
};