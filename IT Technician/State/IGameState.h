#pragma once

#include <array>
#include <map>
#include "../Engine/UI/Colour.h"

enum StateOutcomeType {
	CONTINUE,
	LOAD_STATE,
	RESTORE
};

struct StateOutcome {
	StateOutcomeType type = CONTINUE;
	bool destroy = false;
	int nextState = -1;
	std::map<const char*, const char*> data;
};

class IGameState {
public:
	virtual ~IGameState() = default;

	Colour background = Colour(0, 0, 0);

	virtual void Init();
	virtual void ProcessInput(std::array<bool, 1024> *keys);
	virtual StateOutcome Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Release();

};