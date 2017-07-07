#include "actors.hpp"

using namespace Engine;
using namespace Math;

const float TerrainBlock::blockEdgeLength = Renderer::Window::WINDOW_HEIGHT / 40.0f;

Actor::Actor(GLfloat width, GLfloat height, std::string textureName, vec2<GLfloat>* position) {
	this->width = width;
	this->height = height;
	this->textureName = textureName;
	this->position = position;
}

Actor::~Actor() {
	delete position;
	delete collisionEntity;
}

TerrainBlock::TerrainBlock(TerrainType type, Math::vec2<GLfloat>* position): 
	Actor(6, 6, type == TerrainBalk ? std::string("texture_brick") : std::string("texture_terrain_ladder"),
		position) {
		this->collisionEntity = new Physics::CollisionEntity(this);
}

TerrainBlock::~TerrainBlock() {
}
