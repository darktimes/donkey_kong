#pragma once

#include <vector>
#include <algorithm>
#include "actors.hpp"
#include "physEntity.hpp"

namespace Engine {
	class Actor;
}

namespace Physics {


	class CollisionSet;

	class CollisionEntity {
		public:
			CollisionEntity(Engine::Actor* actor, CollisionSet* container = nullptr);
			virtual ~CollisionEntity();
			CollisionSet* container;
			Engine::Actor* actor;
	};

	class CollisionSet {
		public:
			CollisionSet();
			CollisionSet(std::vector<CollisionEntity*> collisionTargets);
			~CollisionSet();
		
			void addCollisionTarget(CollisionEntity* target);
			void removeCollisionTarget(CollisionEntity* target);
		private:
			std::vector<CollisionEntity*> collisionTargets;
			
	};
}
