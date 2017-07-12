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
			enum CollisionType {COLLISION_CIRCLE, COLLISION_BOX};
			virtual ~CollisionEntity();
			Engine::Actor* actor;
		protected:
			CollisionType collisionType;
		public:

			CollisionSet* container;

			virtual bool collides(CollisionEntity*) = 0;

		protected:

			CollisionEntity(Engine::Actor* actor, CollisionType collisionType, CollisionSet* container = nullptr);
	};

	class BoxCollisionEntity: public CollisionEntity {
		public:
			BoxCollisionEntity(Engine::Actor* actor, CollisionSet* container = nullptr);
			bool collides(CollisionEntity*) override;
	};

	class CircleCollisionEntity: public CollisionEntity {
		public:
			CircleCollisionEntity(Engine::Actor* actor, CollisionSet* container = nullptr);
			bool collides(CollisionEntity*) override;		
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
