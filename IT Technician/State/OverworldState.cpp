#include "OverworldState.h"

#include <GLFW/glfw3.h>
#include <algorithm>

#include "../Engine/ResourceManager.h"

#define MOVEMENT_SPEED 0.4

OverworldState::~OverworldState() {
	delete _renderer;
}

void OverworldState::ProcessInput(std::array<bool, 1024>* keys) {
	if (_isMoving) {
		//only listen for new input if we are not moving
		return;
	}

	auto playerX = (int)_playerX;
	auto playerY = (int)_playerY;

	if ((*keys)[GLFW_KEY_LEFT]) {
		_direction = WEST;
		if (_canMove(-1)) {
			_isMoving = true;
			_movementTimer = MOVEMENT_SPEED;
			_moveMap = (playerX == _center && _x > 0);
		}
	}
	else if ((*keys)[GLFW_KEY_RIGHT]) {
		_direction = EAST;

		if (_canMove(1)) {
			_isMoving = true;
			_movementTimer = MOVEMENT_SPEED;
			_moveMap = (playerX == _center && _x < _mapSize.width - _tileCount);
		}
	}
	else if ((*keys)[GLFW_KEY_UP]) {
		_direction = NORTH;

		if (_canMove(0, -1)) {
			_isMoving = true;
			_movementTimer = MOVEMENT_SPEED;
			_moveMap = (playerY == _center && _y > 0);
		}
	}
	else if ((*keys)[GLFW_KEY_DOWN]) {
		_direction = SOUTH;

		if (_canMove(0, 1)) {
			_isMoving = true;
			_movementTimer = MOVEMENT_SPEED;
			_moveMap = (playerY == _center && _y < _mapSize.height - _tileCount);
		}
	}
}

int OverworldState::Update(double dt) {
	if (!_isMoving) {
		return 0;
	}

	double update = std::min(dt, _movementTimer);
	float movementPartial = update * (1 / MOVEMENT_SPEED);

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

		if (_tiles[_y + _playerY][_x + _playerX].itemType == 5) {
			return 2;
		}
	}

	return 0;
}

void OverworldState::Render() {
	Texture2D tile = ResourceManager::GetTexture("tile");
	Texture2D desk = ResourceManager::GetTexture("desk");
	Texture2D chair = ResourceManager::GetTexture("chair");
	Texture2D laptop = ResourceManager::GetTexture("laptop");
	Texture2D floor = ResourceManager::GetTexture("floor");

	int minX = std::max(0, (int)_x - 1);
	int minY = std::max(0, (int)_y - 1);

	for (int y = minY; y < minY + _tileCount + 2; y++) {
		for (int x = minX; x < minX + _tileCount + 2; x++) {
			if (x >= _mapSize.width || y >= _mapSize.height) {
				continue;
			}

			double xPos = x - _x;
			double yPos = y - _y;
			auto pos = glm::vec2(xPos * _tileSize.x, yPos * _tileSize.y);

			Texture2D texture;
			glm::vec4 textureCoords;
			glm::vec3 colour;

			if (_tiles[y][x].itemType == 1) {
				colour = glm::vec3(1.0f, 0.0f, 0.0f);
				texture = tile;
				textureCoords = tile.GetDefaultCoords();
			}
			else {
				colour = glm::vec3(1.0f);
				texture = floor;

				int offset = ((x % 3) + (y % 3)) * 16;

				textureCoords = glm::vec4(0, 32, offset, offset + 32);
			}


			//draw tile first
			_renderer->DrawSprite(texture, pos, _tileSize, 0.0F, colour, &textureCoords);

			if (_tiles[y][x].itemType <= 1) {
				continue;
			}

			switch (_tiles[y][x].itemType) {
			case 2:
				texture = desk;
				textureCoords = glm::vec4(0, 32, 76, 115);
				break;

			case 3:
				texture = desk;
				textureCoords = glm::vec4(33, 64, 76, 115);
				break;

			case 4:
				texture = desk;
				textureCoords = glm::vec4(65, 96, 76, 115);
				break;

			case 5:
				texture = chair;
				textureCoords = chair.GetDefaultCoords();
				break;

			default:
				continue;
			}

			_renderer->DrawSprite(texture, pos, _tileSize, 0.0F, glm::vec3(1.0f), &textureCoords);

			if (_tiles[y][x].itemType == 3) {
				_renderer->DrawSprite(laptop, glm::vec2((xPos * _tileSize.x) + (_tileSize.x / 3), yPos * _tileSize.y), _tileSize * glm::vec2(0.4f), 0.0f, glm::vec3(1.0));
			}
		}
	}

	_renderPlayer();
}

void OverworldState::Release() {
	ResourceManager::ReleaseTexture("tile");
	ResourceManager::ReleaseTexture("desk");
	ResourceManager::ReleaseTexture("floor");
	ResourceManager::ReleaseTexture("chair");
	ResourceManager::ReleaseTexture("laptop");
}

void OverworldState::Init() {
	background = Colour(0.0f, 0.0f, 0.0f); //Colour(0.58f, 0.3f, 0.0f);

	ResourceManager::LoadTexture("tile", "Resource/Texture/tile.png");
	ResourceManager::LoadTexture("floor", "Resource/Texture/Wood Floor A.png", true);

	ResourceManager::LoadTexture("desk", "Resource/Texture/Desk, Ornate.png", true);

	ResourceManager::LoadTexture("chair", "Resource/Texture/chair.png", true);
	ResourceManager::LoadTexture("laptop", "Resource/Texture/laptop.png", true);

	ResourceManager::LoadTexture("player", "Resource/Texture/player.png", true);

	std::vector<std::vector<unsigned int>> tileData = ResourceManager::LoadMap("Map/Overworld/test.map");
	_tileCount = 9;
	_center = (int)floor(_tileCount / 2);
	_playerX = (float)_center;
	_playerY = (float)_center;

	// calculate dimensions
	_mapSize.height = (float)tileData.size();
	_mapSize.width = (float)tileData[0].size();

	float unitWidth = 594.0f / _tileCount;
	float unitHeight = 792.0f / _tileCount;

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

void OverworldState::_renderPlayer(){
	float frameSpeed = MOVEMENT_SPEED / 7.0f;

	int frame = 0;
	if (_movementTimer > 0) {
		frame = 8 - (std::round((_movementTimer / frameSpeed)));
	}

	int xOffset = frame * 64;
	int yOffset = 512;

	if (_direction == WEST) {
		yOffset += 64;
	}
	else if (_direction == SOUTH) {
		yOffset += 128;
	}
	else if (_direction == EAST) {
		yOffset += 192;
	}

	glm::vec4 textureCoords(xOffset, xOffset + 64, yOffset, yOffset + 64);
	Texture2D player = ResourceManager::GetTexture("player");

	_renderer->DrawSprite(player, glm::vec2((_playerX - 0.5) * _tileSize.x, (_playerY - 1) * _tileSize.y), _tileSize * glm::vec2(2), 0.0F, glm::vec3(1.0f), &textureCoords);

}
