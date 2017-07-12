#include "collisionSet.hpp"

using namespace Physics;

CollisionEntity::CollisionEntity(Engine::Actor* actor, CollisionType collisionType, CollisionSet* container): actor(actor), collisionType(collisionType), container(container) {

}

CollisionEntity::~CollisionEntity() {
	if (actor) {
		actor->collisionEntity = nullptr;
	}
	if (container) {
		container->removeCollisionTarget(this);
	}
}

BoxCollisionEntity::BoxCollisionEntity(Engine::Actor* actor, CollisionSet* contaier): CollisionEntity(actor, COLLISION_BOX, container) {

}

bool BoxCollisionEntity::collides(CollisionEntity* another) {
	return false;
}

CircleCollisionEntity::CircleCollisionEntity(Engine::Actor* actor, CollisionSet* contaier): CollisionEntity(actor, COLLISION_CIRCLE, container) {

}

bool CircleCollisionEntity::collides(CollisionEntity* another) {
	return false;
}

CollisionSet::CollisionSet() {

}

CollisionSet::CollisionSet(std::vector<CollisionEntity*> collisionTargets) {
	this->collisionTargets.insert(collisionTargets.end(), collisionTargets.begin(), collisionTargets.end());
}

CollisionSet::~CollisionSet() {
	for (CollisionEntity* collisionEntity : collisionTargets) {
		collisionEntity->container = nullptr;
		delete collisionEntity;
	}
}

void CollisionSet::addCollisionTarget(CollisionEntity* target) {
	this->collisionTargets.push_back(target);
}

void CollisionSet::removeCollisionTarget(CollisionEntity* target) {
	std::vector<CollisionEntity*>::iterator it = std::find(collisionTargets.begin(), collisionTargets.end(), target);
	collisionTargets.erase(it);
}
