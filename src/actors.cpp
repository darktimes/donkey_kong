#include "actors.hpp"

using namespace Engine;
using namespace Math;

const float TerrainBlock::blockEdgeLength = Renderer::Window::WINDOW_HEIGHT / 40.0f;

Actor::Actor(GLfloat width, GLfloat height,  vec2<GLfloat>* position) {
	this->width = width;
	this->height = height;
	this->position = position;
}

void Actor::draw() {
        glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ResourceManager::textures[getTextureName()]->id);
	Math::mat4 model;
	model = Math::scale(model, TerrainBlock::blockEdgeLength);
	model = Math::translate(model, *terrainBlock->position);
	ResourceManager::shaders["projectionShader"]->use();
	ResourceManager::shaders["projectionShader"]->addUniformMatrix4("model", model);

	glBindVertexArray(Renderer::SpriteRenderer::sprites["halfedQuadratSprite"]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Actor::~Actor() {
	delete position;
	delete collisionEntity;
}

TerrainBlock::TerrainBlock(TerrainType type, Math::vec2<GLfloat>* position):
	Actor(6, 6, position) {
		this->collisionEntity = new Physics::CollisionEntity(this);
		this->type = type;
}

std::string TerrainBlock::getTextureName() {
	return type == TerrainBalk ? std::string("texture_brick") : std::string("texture_terrain_ladder");
}

TerrainBlock::~TerrainBlock() {

}

AnimatedActor::AnimatedActor(GLfloat width, GLfloat height, Math::vec2<GLfloat>* pos): Actor(width, height, pos) {

}

void AnimatedActor::addAnimation(Animation* animation, std::string name) {
	animations.add(name, animation);
}

AnimatedActor::~AnimatedActor() {
	for (auto it : animations) {
		delete it.second;
	}
}

std::string AnimatedActor::getTextureName() {
	return getCurrentAnimation()->getCurrentTexture();
}

Mario::Mario(Math::vec2<Glfloat>* pos): AnimatedActor(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 2, pos) {
	
}
