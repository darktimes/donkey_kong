#include "actors.hpp"
#include "resourceManager.hpp"
#include "utils.hpp"
#include "physEngine.hpp"

using namespace Engine;
using namespace Math;
using namespace Physics;

const float TerrainBlock::blockEdgeLength = Renderer::Window::WINDOW_HEIGHT / 40.0f;

Actor::Actor(GLfloat width, GLfloat height,  vec2<GLfloat>* position) {
	this->width = width;
	this->height = height;
	this->position = position;
	this->collisionEntity = nullptr;
}

void Actor::draw() {
        glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ResourceManager::textures[getTextureName()]->id);
	Math::mat4 model;
	model = Math::scale(model, TerrainBlock::blockEdgeLength);
	model = Math::translate(model, *position);
	ResourceManager::shaders["projectionShader"]->use();
	ResourceManager::shaders["projectionShader"]->addUniformMatrix4("model", model);

	glBindVertexArray(Renderer::SpriteRenderer::sprites["quadratSprite"]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Actor::~Actor() {
	delete position;
	if (collisionEntity) {
		delete collisionEntity;
	}

}

TerrainBlock::TerrainBlock(TerrainType type, Math::vec2<GLfloat>* position):
	Actor(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength, position) {
		this->type = type;
}

std::string TerrainBlock::getTextureName() {
	return type == TerrainBalk ? std::string("terrainBalk") : std::string("terrainLadder");
}

TerrainBlock::~TerrainBlock() {

}

AnimatedActor::AnimatedActor(GLfloat width, GLfloat height, Math::vec2<GLfloat>* pos): Actor(width, height, pos), pEntity(nullptr) {
}

void AnimatedActor::addAnimation(Animation* animation, std::string name) {
	animations[name] = animation;
}

AnimatedActor::~AnimatedActor() {
	for (auto it : animations) {
		delete it.second;
	}
	if (pEntity) {
		delete pEntity;
	}
}

std::string AnimatedActor::getTextureName() {
	return getCurrentAnimation()->getCurrentTexture();
}

Mario::Mario(Math::vec2<GLfloat>* pos): AnimatedActor(TerrainBlock::blockEdgeLength * 0.875f, TerrainBlock::blockEdgeLength * 0.875f, pos) {
	addAnimation(new Animation(std::vector<std::string> {"runRight1", "runRight2"}), "animRunRight");
	addAnimation(new Animation(std::vector<std::string> {"runLeft1", "runLeft2"}), "animRunLeft");
	addAnimation(new Animation(std::vector<std::string> {"climb1", "climb2"}), "animClimb");
	addAnimation(new Animation(std::vector<std::string> {"jumpLeft"}), "animJumpLeft");
	addAnimation(new Animation(std::vector<std::string> {"jumpRight"}), "animJumpRight");
	animState = ANIM_JUMPING_RIGHT;
	face = FACING_RIGHT;
	pEntity = new PhysEntity(this, 50.0f, true);
	setState(JUMPING);
	lostLifeTimer = 0.0f;
}

void AnimatedActor::setState(ActorState state) {
	switch (state) {
		case CLIMBING: jumping = false; atGround = false; climbing = true; break;
		case JUMPING: jumping = true; atGround = false; climbing = false; break;
		case AT_GROUND: jumping = false; atGround = true; climbing = false; break;
	}
}

void Mario::handleAnimationState() {
	if (atGround) {
		if (pEntity->velocity->x > 0) {
			animState = ANIM_RUNNING_RIGHT;
			face = FACING_RIGHT;
			getCurrentAnimation()->stopped = false;
		} else if (pEntity->velocity->x < 0) {
			animState = ANIM_RUNNING_LEFT;
			face = FACING_LEFT;
			getCurrentAnimation()->stopped = false;
		} else {
			switch (face) {
				case FACING_RIGHT: animState = ANIM_RUNNING_RIGHT; break;
				case FACING_LEFT: animState = ANIM_RUNNING_LEFT; break;
			}
			getCurrentAnimation()->stopped = true;
		}
	} else if (climbing) {
		animState = ANIM_CLIMBING;
		getCurrentAnimation()->stopped = false;
		if (pEntity->velocity->y == 0 && pEntity->velocity->x == 0) {
			getCurrentAnimation()->stopped = true;
		}
	} else if (jumping) {
		if (pEntity->velocity->x > 0) {
			animState = ANIM_JUMPING_RIGHT;
			face = FACING_RIGHT;
		} else if (pEntity->velocity->x < 0) {
			animState = ANIM_JUMPING_LEFT;
			face = FACING_LEFT;
		} else {
			switch (face) {
				case FACING_RIGHT: animState = ANIM_JUMPING_RIGHT; break;
				case FACING_LEFT: animState = ANIM_JUMPING_LEFT; break;
			}
			getCurrentAnimation()->stopped = true;
		}
	}
}

Animation* Mario::getCurrentAnimation() {
	switch (animState) {
		case ANIM_RUNNING_RIGHT: return animations["animRunRight"];
		case ANIM_RUNNING_LEFT: return animations["animRunLeft"];
		case ANIM_CLIMBING: return animations["animClimb"];
		case ANIM_JUMPING_RIGHT: return animations["animJumpRight"];
		case ANIM_JUMPING_LEFT: return animations["animJumpLeft"];
		default:
			return nullptr;
	}
}

Barrel::Barrel(Math::vec2<GLfloat>* pos): AnimatedActor(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength, pos) {
		addAnimation(new Animation(std::vector<std::string> {"barrel1", "barrel2", "barrel3", "barrel4"}), "animBarrelRoll");
		addAnimation(new Animation(std::vector<std::string> {"barrelFront1", "barrelFront2"}), "animBarrelRollFront");
		animState = ROLLING;
		pEntity = new PhysEntity(this, 25.0f, true);
		pEntity->velocity->x = 1.5f * Physics::movementDelta;
		setState(JUMPING);
		face = FACING_RIGHT;
		scoreTimer = 0.0f;
}

void Barrel::handleAnimationState() {
	if (climbing) {
		animState = ROLLING_FRONT;
		getCurrentAnimation()->stopped = false;
	} else {
		getCurrentAnimation()->stopped = false;
		animState = ROLLING;
	}
}

Animation* Barrel::getCurrentAnimation() {
	switch (animState) {
		case ROLLING: return animations["animBarrelRoll"];
		case ROLLING_FRONT: return animations["animBarrelRollFront"];
		default:
			return nullptr;
	}
}
