#include "gameSession.hpp"
#include "gameLevel.hpp"
#include <utils.hpp>

using namespace Utils;

GameSession::GameSession() : lifeCount(999999), points(0) {

}

GameSession::~GameSession() {

}

/**
* Sets the current level name and start point bonus.
*/
void GameSession::setLevel(Level* level) {
	this->currentLevelName = level->levelName;
	this->currentBonus = level->startPointBonus;
}
