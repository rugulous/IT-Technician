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

enum Direction {
	NORTH,
	SOUTH,
	EAST,
	WEST
};

class OverworldState : public IGameState {
public:
	OverworldState();
	~OverworldState() override;

	void ProcessInput(std::array<bool, 1024> *keys) override;
	int Update(double dt) override;
	void Render() override;
	void Release() override;

private:
	void _Init() override;

	std::vector<std::vector<Tile>> _tiles;

	SpriteRenderer* _renderer;

	glm::vec2 _tileSize;
	Size _mapSize;
	int _tileCount;
	int _center;

	float _x = 0;
	float _y = 0;

	float _playerX;
	float _playerY;

	bool _isMoving = false;
	Direction _direction = SOUTH;
	double _movementTimer = 0;
	bool _moveMap = true;

	bool _canMove(int xOffset = 0, int yOffset = 0);
};