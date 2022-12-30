#pragma once

#include <array>
#include "../Engine/UI/Colour.h"

class IGameState {
public:
	virtual ~IGameState() = default;

	Colour background = Colour(0, 0, 0);

	virtual void Init();
	virtual void ProcessInput(std::array<bool, 1024> *keys);
	virtual int Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Release();

};