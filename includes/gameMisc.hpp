#pragma once

#include "gameLevel.hpp"

namespace Engine {
	class Level;

	struct GameSession {
		GameSession();
		virtual ~GameSession();
		
		void setLevel(Level* level);

		int lifeCount;
		int points;
		int currentBonus;
		Level* currentLevel;
	};
}
