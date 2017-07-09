#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
			std::vector<TerrainBlock*> ladderBlocks;
			
//			std::vector<AnimatedActor> actors;
			Mario* mario;
			
			IPlayGameStateCallbacks* playGameStateCallbacks;
			
			
			virtual ~Level();
			virtual void draw();
			virtual void processInput(GLFWwindow* window, int key, int scancode, int action, int mode);
			
		protected:
			Level(unsigned startPointBonus, unsigned xBlockCount, unsigned yBlockCount, IPlayGameStateCallbacks* playGameStateCallbacks);
			virtual void initMario() = 0;
	};

	class Level1: public Level {
		public:
			Level1(IPlayGameStateCallbacks* playGameStateCallbacks);
			void draw() override;
			void initMario() override;
			virtual ~Level1();
	};
}


