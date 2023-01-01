#include "OverworldState.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>

#include "../Engine/ResourceManager.h"
#include "../Engine/Util/String.h"

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

	if ((*keys)[GLFW_KEY_R]) {
		_loadMap(_currentMap);
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

		/*if (_tiles[_y + _playerY][_x + _playerX].itemType == 5) {
			return 2;
		}*/
	}

	return 0;
}

void OverworldState::Render() {
	Texture2D tile = ResourceManager::GetTexture("tile");
	Tex floorData = _objectTextures[_floorIndex];
	Texture2D floor = ResourceManager::GetTexture(floorData.texture);

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

			//draw tile first
			if (x > 0 && x < _mapSize.width - 1 && y > 0 && y < _mapSize.height - 1) {
				_renderer->DrawSprite(floor, pos, _tileSize, 0.0F, glm::vec3(1.0f), &floorData.coords);
			}

			for (int l = 0; l < _levelsBelow; l++) {
				int type = _levels[l][y][x];
				if (type == 0 || type >= _objectTextures.size()) {
					continue;
				}

				Tex textureDetails = _objectTextures[type];
				Texture2D texture = ResourceManager::GetTexture(textureDetails.texture);

				_renderer->DrawSprite(texture, pos, _tileSize, 0.0F, glm::vec3(1.0f), &textureDetails.coords);
			}
		}
	}

	_renderPlayer();

	for (int y = minY; y < minY + _tileCount + 2; y++) {
		for (int x = minX; x < minX + _tileCount + 2; x++) {
			if (x >= _mapSize.width || y >= _mapSize.height) {
				continue;
			}

			double xPos = x - _x;
			double yPos = y - _y;
			auto pos = glm::vec2(xPos * _tileSize.x, yPos * _tileSize.y);

			for (int l = _levelsBelow; l < _levels.size(); l++) {
				int type = _levels[l][y][x];
				if (type == 0 || type >= _objectTextures.size()) {
					continue;
				}

				Tex textureDetails = _objectTextures[type];
				Texture2D texture = ResourceManager::GetTexture(textureDetails.texture);

				_renderer->DrawSprite(texture, pos, _tileSize, 0.0F, glm::vec3(1.0f), &textureDetails.coords);
			}
		}
	}
}

void OverworldState::Release() {
	for (Tex data : _objectTextures) {
		ResourceManager::ReleaseTexture(data.texture);
	}

	_objectTextures.clear();

	ResourceManager::ReleaseTexture("player");
}

void OverworldState::Init() {
	background = Colour(0.0f, 0.0f, 0.0f); //Colour(0.58f, 0.3f, 0.0f);

	ResourceManager::LoadTexture("player", "Resource/Texture/player.png", true);

	_loadMap("Map/Overworld/new.map");
	_renderer = new SpriteRenderer();
}

bool OverworldState::_canMove(int xOffset, int yOffset) {
	int x = _x + _playerX + xOffset;
	int y = _y + _playerY + yOffset;

	if (y >= _mapSize.height || y < 0 || x >= _mapSize.width || x < 0) {
		return false;
	}

	return !_solid[y][x];
}

void OverworldState::_renderPlayer() {
	float frameSpeed = MOVEMENT_SPEED / 7.0f;

	int frame = 0;
	if (_movementTimer > 0) {
		frame = 8 - (std::round((_movementTimer / frameSpeed)));
	}

	int xOffset = frame * 64;
	int yOffset = 513;

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


void OverworldState::_loadMap(const std::string& file) {
	Release();
	_objectTextures.clear();
	_levels.clear();
	_solid.clear();

	_isMoving = false;
	_movementTimer = 0;

	_currentMap = file;

	std::vector<std::string> data = Split(ResourceManager::ReadFile(file), '\n');

	//line 1: w,h,floorTexture
	std::vector<std::string> parts = Split(data[0], ',');
	_mapSize.width = atoi(parts[0].c_str());
	_mapSize.height = atoi(parts[1].c_str());
	_floorIndex = atoi(parts[2].c_str());

	//line 2: tiles to display on screen
	_tileCount = atoi(data[1].c_str());

	_center = (int)floor(_tileCount / 2);
	_playerX = (float)_center;
	_playerY = (float)_center;

	float unitWidth = 594.0f / _tileCount;
	float unitHeight = 792.0f / _tileCount;

	_tileSize = glm::vec2(unitWidth, unitHeight);

	//line 3: level count (below,above) 
	parts = Split(data[2], ',');
	_levelsBelow = atoi(parts[0].c_str());
	int totalLevels = _levelsBelow + atoi(parts[1].c_str());

	//line 4: spawn (x,y)
	parts = Split(data[3], ',');
	_x = atoi(parts[0].c_str());
	_y = atoi(parts[1].c_str());

	if (_x < _center) {
		_playerX = _x;
		_x = 0;
	}
	else if (_x + _playerX > _mapSize.width - 2) {
		_playerX = _mapSize.width - _x - _playerX;
	}

	if (_y < _center) {
		_playerY = _y;
		_y = 0;
	}
	else if (_y + _tileCount > _mapSize.height) {
		_y -= (_y + _tileCount) - _mapSize.height;
	}

	std::vector<std::vector<int>> _obj;

	for (int y = 0; y < _mapSize.height; y++) {
		std::vector<bool> row;
		std::vector<int> _objRow;

		for (unsigned int x = 0; x < _mapSize.width; ++x)
		{
			row.push_back(false);
			_objRow.push_back(0);
		}

		this->_solid.push_back(row);
		_obj.push_back(_objRow);
	}

	for (int i = 0; i < totalLevels; i++) {
		_levels.push_back(_obj);
	}

	int numTextures = atoi(data[5].c_str());
	std::map<std::string, std::pair<std::string, bool>> textureBank;

	for (int i = 0; i < numTextures; i++) {
		parts = Split(data[6 + i], ',');
		textureBank.insert(std::pair<std::string, std::pair<std::string, bool>>(parts[0], std::pair<std::string, bool>(parts[1], atoi(parts[2].c_str()))));
	}

	int objStart = 7 + numTextures;
	int objects = atoi(data[objStart].c_str());

	for (int i = 0; i < objects; i++) {
		parts = Split(data[objStart + i + 1], ',');
		Tex t = {
			parts[0],
			glm::vec4(atof(parts[1].c_str()), atof(parts[2].c_str()), atof(parts[3].c_str()), atof(parts[4].c_str()))
		};

		_objectTextures.push_back(t);

		//only load **actually used** textures
		if (textureBank.find(parts[0]) != textureBank.end()) {
			ResourceManager::LoadTexture(parts[0], textureBank[parts[0]].first.c_str(), textureBank[parts[0]].second);
			textureBank.erase(parts[0]);
		}
	}

	int levelStart = objStart + objects + 1;

	for (int i = levelStart; i < data.size(); i++) {
		if (data[i].empty()) {
			continue;
		}

		parts = Split(data[i], ',');

		int level = atoi(parts[0].c_str());
		int x = atoi(parts[1].c_str());
		int y = atoi(parts[2].c_str());
		int w = atoi(parts[3].c_str());
		int h = atoi(parts[4].c_str());
		int type = atoi(parts[5].c_str());
		bool solid = atoi(parts[6].c_str());

		for (int _height = 0; _height < h; _height++) {
			for (int _width = 0; _width < w; _width++) {
				_levels[level][y + _height][x + _width] = type;

				if (solid) {
					_solid[y + _height][x + _width] = true;
				}
			}
		}
	}
}