#include "PipeState.h"

#include "../Engine/ResourceManager.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <random>

void PipeState::Init() {
    _renderer = new SpriteRenderer();

    _textures.push_back(ResourceManager::LoadTexture("h", "Resource/Texture/pipe/horizontal.png", true));
    _textures.push_back(ResourceManager::LoadTexture("v", "Resource/Texture/pipe/vertical.png", true));
    _textures.push_back(ResourceManager::LoadTexture("lb", "Resource/Texture/pipe/left-bottom.png", true));
    _textures.push_back(ResourceManager::LoadTexture("lt", "Resource/Texture/pipe/left-top.png", true));
    _textures.push_back(ResourceManager::LoadTexture("rb", "Resource/Texture/pipe/right-bottom.png", true));
    _textures.push_back(ResourceManager::LoadTexture("rt", "Resource/Texture/pipe/right-top.png", true));

    ResourceManager::LoadTexture("wheel", "Resource/Texture/pipe/wheel.png", true);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, (_textures.size() - 1) * 10);

    for (int y = 0; y < 5; y++) {
        std::vector<std::pair<bool,int>> row;

        for (int x = 0; x < 5; x++) {
            row.push_back(std::pair<bool, int>(false, distr(gen) / 10));
        }

        _pipes.push_back(row);
    }

    _pipes[0][0].first = true;
    _pipes[0][0].second = 1;

    _pipes[1][0].second = 0;
    _pipes[3][4].second = 1;

    _pipes[4][4].first = true;
    _pipes[4][4].second = 1;
}

void PipeState::ProcessInput(std::array<bool, 1024>* keys){
    if (_inputDelay > 0) {
        return;
    }

    if ((*keys)[GLFW_KEY_UP] && _selectedY > 0) {
        _selectedY--;
        _inputDelay = 0.15;
    }
    else if ((*keys)[GLFW_KEY_DOWN] && _selectedY < 4) {
        _selectedY++;
        _inputDelay = 0.15;
    }
    else if ((*keys)[GLFW_KEY_LEFT] && _selectedX > 0) {
        _selectedX--;
        _inputDelay = 0.15;
    }
    else if ((*keys)[GLFW_KEY_RIGHT] && _selectedX < 4) {
        _selectedX++;
        _inputDelay = 0.15;
    }
    else if ((*keys)[GLFW_KEY_ENTER]) {
        std::pair<bool, int> val = _pipes[_selectedY][_selectedX];
        if (val.first || val.second < 0) {
            return;
        }

        bool corner = (val.second >= 2);
        val.second++;

        if (corner) {
            if (val.second >= _textures.size()) {
                val.second = 2;
            }
        }
        else {
            if (val.second >= 2) {
                val.second = 0;
            }
        }
        
        _pipes[_selectedY][_selectedX].second = val.second;
        _inputDelay = 0.15;
    }
}

StateOutcome PipeState::Update(double dt){
    _rotation += dt * 100;

    if (_inputDelay > 0) {
        double update = std::min(_inputDelay, dt);
        _inputDelay -= update;
    }

    return StateOutcome();
}

void PipeState::Render(){
    Texture2D pipe;
    Texture2D wheel = ResourceManager::GetTexture("wheel");

    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            glm::vec2 pos((x * 110) + 20, (y * 110) + 100);

            if (x == _selectedX && y == _selectedY) {
                _renderer->DrawSprite(wheel, pos, glm::vec2(110), _rotation, glm::vec3(0.5f));
            }

            int pipeIndex = _pipes[y][x].second;
            if (pipeIndex < 0) {
                continue;
            }

            glm::vec3 colour = (_pipes[y][x].first) ? glm::vec3(0.7f, 0.3f, 0.2f) : glm::vec3(1.0f);
            pipe = _textures[pipeIndex];

            _renderer->DrawSprite(pipe, pos, glm::vec2(100), 0, colour);
        }
    }
}

void PipeState::Release() {
    delete _renderer;
}
