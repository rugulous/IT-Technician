#pragma once

#include "IGameState.h"

#include <vector>

#include "../Engine/UI/SpriteRenderer.h"

class PipeState : public IGameState {
public:
	void Init() override;
	void ProcessInput(std::array<bool, 1024>* keys);
	StateOutcome Update(double dt) override;
	void Render() override;
	void Release() override;

private:
	SpriteRenderer* _renderer;

	std::vector<std::vector<std::pair<bool, int>>> _pipes;
	std::vector<Texture2D> _textures;

	int _selectedX = 0;
	int _selectedY = 0;

	float _rotation = 0;

	double _inputDelay = 0;
};