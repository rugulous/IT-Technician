#pragma once

class IGameState {
public:
	virtual ~IGameState() = default;

	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
};