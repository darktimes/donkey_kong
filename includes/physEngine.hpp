#pragma once

#include <vector>

#include "physEntity.hpp"
#include "gameLevel.hpp"
#include "actors.hpp"

namespace Physics {
	
	class PhysLevel {
		public:
			PhysLevel(Engine::Level level);
			~PhysLevel();
			CollisionSet terrainCollisionTargets;
			CollisionSet barrelCollisionTargets;
			CollisionSet ladderCollisionTargets;
			
			Mario* mario;
	};

	class PhysEngine {
		public:
			static void init();
			static void tick(GLfloat time);
			static void loadLevel(Engine::Level level);
			static void clear();

			~PhysEngine();
			
		private:
			static PhysEngine* instance;
			GLfloat lastTime;
			PhysEngine();
			void detectCollisions();
//			void resolveReflections();
			void moveEntities();
	};
}
