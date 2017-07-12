#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <set>

#include "gameStates.hpp"
#include "actors.hpp"


namespace Engine {

	class IPlayGameStateCallbacks;

	struct Ladder {
		Ladder(Math::vec2<GLfloat>,Math::vec2<GLfloat>);
		Math::vec2<GLfloat> location;
		Math::vec2<GLfloat> dimensions;
	};

	class Level {
		public:
			static  Level* generateLevel1(IPlayGameStateCallbacks* playGameStateCallbacks);

			unsigned startPointBonus;
			unsigned xBlockCount;
			unsigned yBlockCount;

			std::set<int> keySet;
			std::vector<TerrainBlock*> terrainBlocks;
//			std::vector<TerrainBlock*> ladderBlocks;
			std::vector<Ladder*> ladders;

//			std::vector<AnimatedActor> actors;
			Mario* mario;

			IPlayGameStateCallbacks* playGameStateCallbacks;

			bool canClimb(AnimatedActor* actor);

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
