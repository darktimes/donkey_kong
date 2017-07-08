#include "gameMisc.hpp"
#include "gameLevel.hpp"

using namespace Engine;

GameSession::GameSession() : lifeCount(3), points(0), currentBonus(0) {
	currentLevel = nullptr;
}

GameSession::~GameSession() {

}

void GameSession::setLevel(Level* level) {
	if (currentLevel) {
		delete currentLevel;
	}
	currentLevel = level;
	currentBonus = currentLevel->startPointBonus;
}
