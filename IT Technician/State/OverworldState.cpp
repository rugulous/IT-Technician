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
	if (keys[GLFW_KEY_LEFT] && _x > 0) {
		_x -= 0.01;
	}
	
	if (keys[GLFW_KEY_RIGHT] && _x < _mapSize.width - 9) {
		_x += 0.01;
	}

	if (keys[GLFW_KEY_UP] && _y > 0) {
		_y -= 0.01;
	}

	if (keys[GLFW_KEY_DOWN] && _y < _mapSize.height - 9) {
		_y += 0.01;
	}
}

int OverworldState::Update(double dt) {
	return 0;
}

void OverworldState::Render() {
	glm::vec2 offset(_x * _tileSize.width, _y * _tileSize.height);

	for (GameObject& tile : this->_tiles) {
		tile.Draw(*_renderer, offset);
	}

	_player->Draw(*_renderer);
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

	glm::vec2 size(unitWidth, unitHeight);

	for (unsigned int y = 0; y < _mapSize.height; ++y)
	{
		for (unsigned int x = 0; x < _mapSize.width; ++x)
		{
			glm::vec2 pos((unitWidth * x) + offsetY, (unitHeight * y) + offsetX);

			auto colour = glm::vec3(0.58f, 0.3f, 0.0f);

			if (tileData[y][x] == 1) // solid
			{
				colour = glm::vec3(1.0f, 0.0f, 0.0f);
			}

			this->_tiles.emplace_back(pos, size, "tile", colour);
		}
	}

	_player = new GameObject(glm::vec2(4.0f * unitWidth, 4.0f * unitHeight), size, "tile", glm::vec3(0.0f, 1.0f, 0.0f));
	_renderer = new SpriteRenderer();

	_tileSize.width = unitWidth;
	_tileSize.height = unitHeight;
}