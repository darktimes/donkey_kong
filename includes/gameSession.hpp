#pragma once

#include <memory>
#include "gameLevel.hpp"

class Level;

/**
* GameSession struct stores game progress relevant data.
*/
struct GameSession {
	GameSession();
	virtual ~GameSession();

	void setLevel(Level*);

	int lifeCount;
	int points;
	int currentBonus;
	std::string currentLevelName;
};
