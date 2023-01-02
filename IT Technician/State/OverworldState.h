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
	int itemType;
};

enum Direction {
	NORTH,
	SOUTH,
	EAST,
	WEST
};

struct Tex {
	std::string texture;
	glm::vec4 coords;
};

class OverworldState : public IGameState {
public:
	~OverworldState() override;

	void Init() override;
	void ProcessInput(std::array<bool, 1024> *keys) override;
	StateOutcome Update(double dt) override;
	void Render() override;
	void Release() override;

private:
	int _labours = 0;

	std::vector<std::vector<bool>> _solid;
	std::vector<std::vector<std::vector<int>>> _levels;
	std::vector<std::vector<StateOutcome*>> _outcomes;
	std::vector<Tex> _objectTextures;
	int _floorIndex = 0;
	int _levelsBelow = 0;
	std::string _currentMap = "";

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
	bool _returning = false;
	Direction _direction = SOUTH;
	double _movementTimer = 0;
	double _attackTimer = 0;
	bool _moveMap = true;

	void _loadMap(const std::string& map);

	bool _canMove(int xOffset = 0, int yOffset = 0);
	void _renderPlayer(glm::vec3 colour);
};