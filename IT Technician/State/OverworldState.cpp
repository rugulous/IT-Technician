#include "OverworldState.h"

#include <GLFW/glfw3.h>
#include <algorithm>

#include "../Engine/ResourceManager.h"

OverworldState::OverworldState() {
	this->_Init();
}

OverworldState::~OverworldState() {
	delete _renderer;
}

void OverworldState::ProcessInput(std::array<bool, 1024>* keys) {
	if (_isMoving) {
		//only listen for new input if we are not moving
		return;
	}

	if ((*keys)[GLFW_KEY_LEFT] && _canMove(-1)) {
		_isMoving = true;
		_direction = WEST;
		_movementTimer = 0.2;
		_moveMap = (_playerX == _center && _x > 0);
	}
	else if ((*keys)[GLFW_KEY_RIGHT] && _canMove(1)) {
		_isMoving = true;
		_direction = EAST;
		_movementTimer = 0.2;
		_moveMap = (_playerX == _center && _x < _mapSize.width - _tileCount);
	}
	else if ((*keys)[GLFW_KEY_UP] && _canMove(0, -1)) {
		_isMoving = true;
		_direction = NORTH;
		_movementTimer = 0.2;
		_moveMap = (_playerY == _center && _y > 0);
	}
	else if ((*keys)[GLFW_KEY_DOWN] && _canMove(0, 1)) {
		_isMoving = true;
		_direction = SOUTH;
		_movementTimer = 0.2;
		_moveMap = (_playerY == _center && _y < _mapSize.height - _tileCount);
	}
}

int OverworldState::Update(double dt) {
	if (!_isMoving) {
		return 0;
	}

	double update = std::min(dt, _movementTimer);
	float movementPartial = update * 5;

	if (_direction == NORTH) {
		if (_moveMap) {
			_y -= movementPartial;
		}
		else {
			_playerY -= movementPartial;
		}
	}
	else if (_direction == SOUTH) {
		if (_moveMap) {
			_y += movementPartial;
		}
		else {
			_playerY += movementPartial;
		}
	}
	else if (_direction == EAST) {
		if (_moveMap) {
			_x += movementPartial;
		}
		else {
			_playerX += movementPartial;
		}
	}
	else if (_direction == WEST) {
		if (_moveMap) {
			_x -= movementPartial;
		}
		else {
			_playerX -= movementPartial;
		}
	}

	_movementTimer -= update;
	if (_movementTimer <= 0) {
		_isMoving = false;
		_movementTimer = 0;

		if (_moveMap) {
			_x = round(_x);
			_y = round(_y);
		}
		else {
			_playerX = round(_playerX);
			_playerY = round(_playerY);
		}
	}

	return 0;
}

void OverworldState::Render() {
	Texture2D tile = ResourceManager::GetTexture("tile");
	Texture2D deskA = ResourceManager::GetTexture("deska");
	Texture2D deskB = ResourceManager::GetTexture("deskb");
	Texture2D deskC = ResourceManager::GetTexture("deskc");
	Texture2D chair = ResourceManager::GetTexture("chair");

	int minX = std::max(0, (int)_x - 1);
	int minY = std::max(0, (int)_y - 1);

	for (int y = minY; y < minY + _tileCount + 2; y++) {
		for (int x = minX; x < minX + _tileCount + 2; x++) {
			if (x >= _mapSize.width || y >= _mapSize.height) {
				continue;
			}

			double xPos = x - _x;
			double yPos = y - _y;

			glm::vec3 colour = glm::vec3(1.0f);

			Texture2D texture;
			switch (_tiles[y][x].itemType) {
			case 2:
				texture = deskA;
				break;

			case 3:
				texture = deskB;
				break;

			case 4:
				texture = deskC;
				break;

			case 5:
				texture = chair;
				break;

			default:
				texture = tile;
				colour = glm::vec3(0.58f, 0.3f, 0.0f);
			}

			if (_tiles[y][x].itemType == 1) {
				colour = glm::vec3(1.0f, 0.0f, 0.0f);
			}

			_renderer->DrawSprite(texture, glm::vec2(xPos * _tileSize.x, yPos * _tileSize.y), _tileSize, 0.0F, colour);
		}
	}

	_renderer->DrawSprite(tile, glm::vec2(_playerX * _tileSize.x, _playerY * _tileSize.y), _tileSize, 0.0F, glm::vec3(0.0F, 1.0F, 0.0F));
}

void OverworldState::Release() {
	ResourceManager::ReleaseTexture("tile");
	ResourceManager::ReleaseTexture("deska");
	ResourceManager::ReleaseTexture("deskb");
	ResourceManager::ReleaseTexture("deskc");
	ResourceManager::ReleaseTexture("chair");
}

void OverworldState::_Init() {
	ResourceManager::LoadTexture("tile", "Resource/Texture/tile.png");
	
	ResourceManager::LoadTexture("deska", "Resource/Texture/DeskA.png", true);
	ResourceManager::LoadTexture("deskb", "Resource/Texture/DeskB.png", true);
	ResourceManager::LoadTexture("deskc", "Resource/Texture/DeskC.png", true);

	ResourceManager::LoadTexture("chair", "Resource/Texture/chair.png", true);

	std::vector<std::vector<unsigned int>> tileData = ResourceManager::LoadMap("Map/Overworld/test.map");
	_tileCount = 9;
	_center = floor(_tileCount / 2);
	_playerX = _center;
	_playerY = _center;

	// calculate dimensions
	_mapSize.height = tileData.size();
	_mapSize.width = tileData[0].size();

	float unitWidth = 594 / _tileCount;
	float unitHeight = 792 / _tileCount;

	int offsetX = 0;
	int offsetY = 0;

	if (_mapSize.height < _tileCount) {
		offsetX = ((_tileCount - _mapSize.height) / 2) * unitHeight;
	}

	if (_mapSize.width < _tileCount) {
		offsetY = ((_tileCount - _mapSize.width) / 2) * unitWidth;
	}

	_tileSize = glm::vec2(unitWidth, unitHeight);

	for (unsigned int y = 0; y < _mapSize.height; ++y)
	{
		std::vector<Tile> row;

		for (unsigned int x = 0; x < _mapSize.width; ++x)
		{
			Tile tile;
			tile.isSolid = (tileData[y][x] > 0 && tileData[y][x] < 5);
			tile.itemType = tileData[y][x];

			row.push_back(tile);
		}

		this->_tiles.push_back(row);
	}

	_renderer = new SpriteRenderer();
}

bool OverworldState::_canMove(int xOffset, int yOffset) {
	int x = _x + _playerX + xOffset;
	int y = _y + _playerY + yOffset;

	return !_tiles[y][x].isSolid;
}