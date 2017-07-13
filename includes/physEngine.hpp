#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "physEntity.hpp"
#include "gameLevel.hpp"
#include "actors.hpp"
#include "math.hpp"
#include "renderer.hpp"

namespace Engine {
	class Level;
}

namespace Physics {

	enum Direction {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
	const GLfloat movementDelta = 90.0f;

	typedef std::tuple<GLboolean, Direction, Engine::TerrainBlock*, Math::vec2<GLfloat>> Collision;

	class PhysLevel {
		public:
			PhysLevel(Engine::Level* level);
			~PhysLevel();
			CollisionSet* terrainCollisionTargets;
			CollisionSet* barrelCollisionTargets;
			CollisionSet* ladderCollisionTargets;

			Engine::Level* level;
			Engine::Mario* mario;

	};

	extern GLfloat collisionBias;

	class PhysEngine {
		public:
			static void init(Engine::Level* level);
			static void tick();
			static void clear();
			static void togglePause();

			~PhysEngine();

		private:
			static PhysEngine* instance;

			PhysLevel* currentPhysLevel;
			double lastTime;
			PhysEngine();
			void processCollisions();
			bool buttonPressed(int key);
			void eraseButton(int key);
//			void resolveReflections();
			void moveEntities();

			Direction vectorDirection(Math::vec2<GLfloat> target);
			bool pause;
	};
}
