#pragma once

#include <array>

class IGameState {
public:
	virtual ~IGameState() = default;

	virtual void ProcessInput(std::array<bool, 1024> *keys);
	virtual int Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Release();

private:
	virtual void _Init();
};