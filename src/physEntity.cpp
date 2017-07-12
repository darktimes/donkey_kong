#include "physEntity.hpp"

using namespace Physics;

PhysEntity::PhysEntity(Engine::Actor* actor, GLfloat weight, bool ged): actor(actor), weight(weight), ged(ged) {
	velocity = new Math::vec2<GLfloat>(0.0f, 0.0f);
	acceleration =  new Math::vec2<GLfloat>(0.0f, 0.0f);
}

PhysEntity::~PhysEntity() {
	delete velocity;
	delete acceleration;
}
