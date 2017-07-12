#include "physEngine.hpp"
#include "utils.hpp"
#include <cstdlib>

using namespace Physics;
using namespace Engine;
using namespace Utils;

PhysEngine* PhysEngine::instance = nullptr;

PhysLevel::PhysLevel(Engine::Level* level): level(level), mario(level->mario) {
}

PhysLevel::~PhysLevel() {
	mario = nullptr;
	delete terrainCollisionTargets;
	delete barrelCollisionTargets;
	delete ladderCollisionTargets;
}

void PhysEngine::init(Engine::Level* level) {
	if (!PhysEngine::instance) {
		PhysEngine::instance = new PhysEngine();
	}
	PhysEngine::instance->currentPhysLevel = new PhysLevel(level);
}

void PhysEngine::tick() {
	if (PhysEngine::instance) {
		PhysEngine::instance->processCollisions();
		PhysEngine::instance->moveEntities();
	}
}

void PhysEngine::clear() {
	if (PhysEngine::instance) {
		delete PhysEngine::instance->currentPhysLevel;
		PhysEngine::instance->currentPhysLevel = nullptr;
	}
}

void PhysEngine::togglePause() {
	PhysEngine::instance->pause = !PhysEngine::instance->pause;
}

PhysEngine::PhysEngine(): currentPhysLevel(nullptr), lastTime(0.0f), pause(false){
}

PhysEngine::~PhysEngine() {
		delete PhysEngine::instance->currentPhysLevel;
		PhysEngine::instance->currentPhysLevel = nullptr;
}

void PhysEngine::processCollisions() {

}

void PhysEngine::moveEntities() {
	if (pause) {
		return;
	}
	if (lastTime == 0.0f) {
		lastTime = glfwGetTime();
		return;
	}

	double elapsedTime = glfwGetTime() - lastTime;
	Engine::Mario* mario = currentPhysLevel->level->mario;

	mario->position->x += mario->pEntity->velocity->x * elapsedTime;
	mario->position->y += mario->pEntity->velocity->y * elapsedTime;

	std::vector<Collision> marioCollisions;
	bool hasGroundBeneath = false;
	for (TerrainBlock* terrainBlock: currentPhysLevel->level->terrainBlocks) {
		if (terrainBlock->type == Engine::TerrainBalk) {
			if (std::get<0>(checkMarioCollision(mario, terrainBlock, true))) {
					hasGroundBeneath = true;
			}
			Collision current = checkMarioCollision(mario, terrainBlock, false);
			if (std::get<0>(current)) {
				marioCollisions.push_back(current);
			}
		}
	}

	if (mario->climbing) {

	} else if (mario->jumping) {
		mario->pEntity->velocity->y -= 360.0f * elapsedTime;
		for (Collision collision: marioCollisions) {
			Direction collisionDirection = std::get<1>(collision);
			TerrainBlock* terrainBlock = std::get<2>(collision);
			Math::vec2<GLfloat> collisionVelocity = std::get<3>(collision);
			if (collisionDirection == RIGHT || collisionDirection == LEFT) {
					// if (Math::dot(*collisionVelocity.normalize(), Math::vec2<GLfloat>(0.0f, 1.0f)) >
					// 	Math::dot(*collisionVelocity.normalize(), Math::vec2<GLfloat>(0.0f, -1.0f)))
					// 	{
						// GLfloat yDelta = abs(terrainBlock->position->y + terrainBlock->height - mario->position->y);
						// if (yDelta <= TerrainBlock::blockEdgeLength / 7.0f) {
						// 	Logger::i("x");
						// 	mario->position->y +=yDelta;
						// } else {
							mario->pEntity->velocity->x = 0.0f;
						// }
					if (currentPhysLevel->level->keySet.find(GLFW_KEY_A) != currentPhysLevel->level->keySet.end()) {
						currentPhysLevel->level->keySet.erase(currentPhysLevel->level->keySet.find(GLFW_KEY_A));
					}
					if (currentPhysLevel->level->keySet.find(GLFW_KEY_D) != currentPhysLevel->level->keySet.end()) {
						currentPhysLevel->level->keySet.erase(currentPhysLevel->level->keySet.find(GLFW_KEY_D));
					}
				// }
			} else if (collisionDirection == UP) {
				mario->position->y = terrainBlock->position->y + terrainBlock->height;
				mario->pEntity->velocity->x = 0.0f;
				mario->pEntity->velocity->y = 0.0f;
				mario->setState(Mario::AT_GROUND);
				std::set<int>::iterator a_key = currentPhysLevel->level->keySet.find(GLFW_KEY_A);
				std::set<int>::iterator d_key = currentPhysLevel->level->keySet.find(GLFW_KEY_D);
				if (a_key != currentPhysLevel->level->keySet.end()) {
					mario->pEntity->velocity->x -= Physics::movementDelta;
				}
				if (d_key != currentPhysLevel->level->keySet.end()) {
					mario->pEntity->velocity->x += Physics::movementDelta;
				}
			}
		}
	} else if (mario->atGround) {
		if (!hasGroundBeneath) {
			mario->setState(Mario::JUMPING);
		} else {
			for (Collision collision: marioCollisions) {
				Direction collisionDirection = std::get<1>(collision);
				if (collisionDirection == LEFT || collisionDirection == RIGHT) {
					TerrainBlock* terrainBlock = std::get<2>(collision);
					GLfloat yDelta = (terrainBlock->position->y + terrainBlock->height - mario->position->y);
					// Utils::Logger::i("delta: " + std::to_string(yDelta) + ", deltaMax: " + std::to_string(TerrainBlock::blockEdgeLength / 7.0f));
					if (yDelta <= (GLuint)(TerrainBlock::blockEdgeLength / 7.0f) + 1) {
						mario->position->y += yDelta;

					} else {
						mario->pEntity->velocity->x = 0.0f;
						if (currentPhysLevel->level->keySet.find(GLFW_KEY_A) != currentPhysLevel->level->keySet.end()) {
							currentPhysLevel->level->keySet.erase(currentPhysLevel->level->keySet.find(GLFW_KEY_A));
						}
						if (currentPhysLevel->level->keySet.find(GLFW_KEY_D) != currentPhysLevel->level->keySet.end()) {
							currentPhysLevel->level->keySet.erase(currentPhysLevel->level->keySet.find(GLFW_KEY_D));
						}
					}
				}
			}
		}
		// if (!hasGroundBeneath) {
		// 	mario->setState(Mario::JUMPING);
		// }
	}

	lastTime = glfwGetTime();
}

Collision PhysEngine::checkMarioCollision(Engine::Mario* mario, Engine::TerrainBlock* terrainBlock, bool exact) {
	// bool x = (mario->position->x >= terrainBlock->position->x + 1.0f)

	bool x = (mario->position->x >= terrainBlock->position->x && mario->position->x + mario->width >= terrainBlock->position->x) ||
	 terrainBlock->position->x + terrainBlock->width >= mario->position->x;
	bool y;
	if (mario->jumping) {
		y = mario->position->y <= terrainBlock->position->y + terrainBlock->height + (exact ? 0.0f : 1.0f) && mario->position->y >= terrainBlock->position->y + terrainBlock->height - (exact ? 0.0f : 1.0f);
	} else if (mario->atGround) {
		y = mario->position->y + mario->height >= terrainBlock->position->y && mario->position->y <= terrainBlock->position->y + terrainBlock->height;
	}
	return Collision(x && y, vectorDirection(*mario->pEntity->velocity), terrainBlock, Math::vec2<GLfloat>(*mario->pEntity->velocity));
}

Direction PhysEngine::vectorDirection(Math::vec2<GLfloat> target)
{
    Math::vec2<GLfloat> compass[] = {
			Math::vec2<GLfloat>(0.0f, 1.0f),	// up
			Math::vec2<GLfloat>(1.0f, 0.0f),	// right
			Math::vec2<GLfloat>(0.0f, -1.0f),	// down
			Math::vec2<GLfloat>(-1.0f, 0.0f)
		};
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = Math::dot(*target.normalize(), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}
