#include "OverworldState.h"

#include <algorithm>

#include "../Engine/ResourceManager.h"

OverworldState::OverworldState() {
	this->_Init();
}

OverworldState::~OverworldState() {
	delete _renderer;
}

int OverworldState::Update(double dt) {
	return 0;
}

void OverworldState::Render() {
	for (GameObject& tile : this->_tiles) {
		tile.Draw(*_renderer);
	}
}

void OverworldState::_Init() {
	std::vector<std::vector<unsigned int>> tileData = ResourceManager::LoadMap("Map/Overworld/test.map");

	// calculate dimensions
	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size();

	float unitWidth = 600 / 10;
	float unitHeight = 800 / 10;

	int offsetX = 0;
	int offsetY = 0;

	if (height < 10) {
		offsetX = ((10 - height) / 2) * unitHeight;
	}

	if (width < 10) {
		offsetY = ((10 - width) / 2) * unitWidth;
	}

	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			glm::vec2 pos((unitWidth * x) + offsetY, (unitHeight * y) + offsetX);
			glm::vec2 size(unitWidth, unitHeight);

			auto colour = glm::vec3(0.58f, 0.3f, 0.0f);

			if (tileData[y][x] == 1) // solid
			{
				colour = glm::vec3(1.0f, 0.0f, 0.0f);
			}

			this->_tiles.emplace_back(pos, size, "tile", colour);
		}
	}

	_renderer = new SpriteRenderer();
}