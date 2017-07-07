#pragma once

#include <iostream>
#include <vector>

#include "gameStates.hpp"
#include "actors.hpp"


namespace Engine {

	class IPlayGameStateCallbacks;

	class Level {
		public:
			static  Level* generateLevel1(IPlayGameStateCallbacks* playGameStateCallbacks);
		
			unsigned startPointBonus;
			unsigned xBlockCount;
			unsigned yBlockCount;
			
			
			std::vector<TerrainBlock*> terrainBlocks;
			
//			std::vector<AnimatedActor> actors;
			Mario* mario;
			
			IPlayGameStateCallbacks* playGameStateCallbacks;
			
			
			virtual ~Level();
			virtual void draw();
			virtual void processInput(GLFWwindow* window, int key, int scancode, int action, int mode);
			
		private:
			Level(unsigned startPointBonus, unsigned xBlockCount, unsigned yBlockCount, IPlayGameStateCallbacks* playGameStateCallbacks);
	};

}


