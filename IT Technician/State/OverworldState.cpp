#include "OverworldState.h"

#include <GLFW/glfw3.h>
#include <algorithm>

#include "../Engine/ResourceManager.h"

OverworldState::OverworldState() {
	this->_Init();
}

OverworldState::~OverworldState() {
	delete _player;
	delete _renderer;
}

void OverworldState::ProcessInput(std::array<bool, 1024> keys) {
	if (keys[GLFW_KEY_LEFT] && _canMove(-1)) {
		if (_playerX > 4) {
			_playerX -= 0.01;
		}
		else if (_x > 0) {
			_x -= 0.01;
		}
		else if (_playerX > 0) {
			_playerX -= 0.01;
		}
	}

	if (keys[GLFW_KEY_RIGHT] && _canMove(1)) {
		if (_playerX < 4) {
			_playerX += 0.01;
		}
		else if (_x < _mapSize.width - 9) {
			_x += 0.01;
		}
		else if (_playerX < 8) {
			_playerX += 0.01;
		}
	}

	if (keys[GLFW_KEY_UP] && _canMove(0, -1)) {
		if (_playerY > 4) {
			_playerY -= 0.01;
		}
		else if (_y > 0) {
			_y -= 0.01;
		}
		else if (_playerY > 0) {
			_playerY -= 0.01;
		}
	}

	if (keys[GLFW_KEY_DOWN] && _canMove(0, 1)) {
		if (_playerY < 4) {
			_playerY += 0.01;
		}
		else if (_y < _mapSize.height - 9) {
			_y += 0.01;
		}
		else if (_playerY < 8) {
			_playerY += 0.01;
		}
	}
}

int OverworldState::Update(double dt) {
	return 0;
}

void OverworldState::Render() {
	Texture2D tile = ResourceManager::GetTexture("tile");

	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			glm::vec3 colour = (_tiles[y + (int)_y][x + (int)_x].isSolid) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.58f, 0.3f, 0.0f);

			_renderer->DrawSprite(tile, glm::vec2(x * _tileSize.x, y * _tileSize.y), _tileSize, 0.0F, colour);
		}
	}

	_renderer->DrawSprite(tile, glm::vec2(_playerX * _tileSize.x, _playerY * _tileSize.y), _tileSize, 0.0F, glm::vec3(0.0F, 1.0F, 0.0F));
}

void OverworldState::_Init() {
	std::vector<std::vector<unsigned int>> tileData = ResourceManager::LoadMap("Map/Overworld/test.map");

	// calculate dimensions
	_mapSize.height = tileData.size();
	_mapSize.width = tileData[0].size();

	int maxSize = 9;

	float unitWidth = 594 / maxSize;
	float unitHeight = 792 / maxSize;

	int offsetX = 0;
	int offsetY = 0;

	if (_mapSize.height < maxSize) {
		offsetX = ((maxSize - _mapSize.height) / 2) * unitHeight;
	}

	if (_mapSize.width < maxSize) {
		offsetY = ((maxSize - _mapSize.width) / 2) * unitWidth;
	}

	_tileSize = glm::vec2(unitWidth, unitHeight);

	for (unsigned int y = 0; y < _mapSize.height; ++y)
	{
		std::vector<Tile> row;

		for (unsigned int x = 0; x < _mapSize.width; ++x)
		{
			Tile tile;
			tile.isSolid = (tileData[y][x] == 1);

			row.push_back(tile);
		}

		this->_tiles.push_back(row);
	}

	_player = new GameObject(glm::vec2(4.0f * unitWidth, 4.0f * unitHeight), _tileSize, "tile", glm::vec3(0.0f, 1.0f, 0.0f));
	_renderer = new SpriteRenderer();
}

bool OverworldState::_canMove(int xOffset, int yOffset) {
	int x = floor(_x) + floor(_playerX) + xOffset;
	int y = floor(_y) + floor(_playerY) + yOffset;

	return !_tiles[y][x].isSolid;
}