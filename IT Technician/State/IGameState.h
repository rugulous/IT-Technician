#pragma once

class IGameState {
public:
	virtual ~IGameState() = default;

	virtual int Update(double dt) = 0;
	virtual void Render() = 0;

private:
	virtual void _Init() = 0;
};