#pragma once

#include "gameLevel.hpp"

namespace Engine {
	class Level;

	struct GameSession {
		GameSession();
		virtual ~GameSession();
		
		int lifeCount;
		int points;
		Level* currentLevel;
	};
}
