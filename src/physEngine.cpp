#include "physEngine.hpp"
#include "utils.hpp"
#include <cstdlib>

using namespace Physics;
using namespace Engine;
using namespace Utils;

PhysEngine* PhysEngine::instance = nullptr;

GLfloat Physics::collisionBias = 2.0f;

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

bool PhysEngine::buttonPressed(int key) {
	return currentPhysLevel->level->keySet.find(key) != currentPhysLevel->level->keySet.end();
}

void PhysEngine::eraseButton(int key) {
	if (buttonPressed(key)) {
		currentPhysLevel->level->keySet.erase(currentPhysLevel->level->keySet.find(key));
	}
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

	bool x_touched_left_window = mario->position->x <= 0.0f && mario->position->x >= -collisionBias;
	bool x_touched_right_window = mario->position->x + mario->width <= Renderer::Window::WINDOW_WIDTH + collisionBias && mario->position->x + mario->width >= Renderer::Window::WINDOW_WIDTH;

	// std::vector<Collision> marioCollisions;
	// bool hasGroundBeneath = false;

	TerrainBlock* groundBlock = nullptr;
	TerrainBlock* touchedRightBlock = nullptr;
	TerrainBlock* touchedLeftBlock = nullptr;

	for (TerrainBlock* terrainBlock: currentPhysLevel->level->terrainBlocks) {
		bool x_collided = (mario->position->x <= terrainBlock->position->x + terrainBlock->width && mario->position->x + mario->width >= terrainBlock->position->x);
		bool y_collided = mario->position->y + mario->height >= terrainBlock->position->y && mario->position->y <= terrainBlock->position->y + terrainBlock->height;
		if (terrainBlock->type == Engine::TerrainBalk) {
			bool x_touched_right = mario->position->x + mario->width >= terrainBlock->position->x && mario->position->x + mario->width <= terrainBlock->position->x + collisionBias;
			bool x_touched_left = mario->position->x >= terrainBlock->position->x + terrainBlock->width - collisionBias && mario->position->x <= terrainBlock->position->x + terrainBlock->width;
			bool y_touched_up = mario->position->y <= terrainBlock->position->y + terrainBlock->height && mario->position->y >= terrainBlock->position->y + terrainBlock->height - collisionBias;

			if (y_touched_up && x_collided) {
				groundBlock = terrainBlock;
			}
			if (y_collided && x_touched_left) {
				touchedLeftBlock = terrainBlock;
			} else if (y_collided && x_touched_right) {
				touchedRightBlock = terrainBlock;
			}
		}
	}

	bool canClimb = false;

	for (Ladder* ladder : currentPhysLevel->level->ladders) {
		bool x_collided = mario->position->x + mario->width <= ladder->location.x + ladder->dimensions.x && mario->position->x >= ladder->location.x;
		bool y_collided = mario->position->y >= ladder->location.y && mario->position->y + mario->height <= ladder->location.y + ladder->dimensions.y;
		if (x_collided && y_collided) {
			canClimb = true;
		}
	}
Logger::i(std::to_string(canClimb));
	if (mario->jumping) {
		if (groundBlock) {
			mario->setState(Mario::AT_GROUND);
			mario->pEntity->velocity->y = 0.0f;
			mario->position->y = groundBlock->position->y + groundBlock->height;
			if (mario->pEntity->velocity->x > 0) {
				if (buttonPressed(GLFW_KEY_D)) {
					if (!buttonPressed(GLFW_KEY_A)) {
						mario->pEntity->velocity->x = Physics::movementDelta;
					} else {
						mario->pEntity->velocity->x = 0.0f;
					}
				} else {
					mario->pEntity->velocity->x = 0.0f;
				}
			} else if (mario->pEntity->velocity->x < 0) {
				if (buttonPressed(GLFW_KEY_A)) {
					if (!buttonPressed(GLFW_KEY_D)) {
						mario->pEntity->velocity->x = -Physics::movementDelta;
					} else {
						mario->pEntity->velocity->x = 0.0f;
					}
				} else {
					mario->pEntity->velocity->x = 0.0f;
				}
			}
		} else {
			if (x_touched_left_window) {
				mario->position->x = 0.0f;
				eraseButton(GLFW_KEY_A);
				mario->pEntity->velocity->x = 0.0f;
			} else if (x_touched_right_window) {
				mario->position->x = Renderer::Window::WINDOW_WIDTH - mario->width;
				eraseButton(GLFW_KEY_D);
				mario->pEntity->velocity->x = 0.0f;
			}
			if (buttonPressed(GLFW_KEY_A)) {
				if (!buttonPressed(GLFW_KEY_D)) {
					mario->pEntity->velocity->x = -Physics::movementDelta;
				} else {
					mario->pEntity->velocity->x = 0.0f;
				}
			} else if (buttonPressed(GLFW_KEY_D)) {
				if (!buttonPressed(GLFW_KEY_A)) {
					mario->pEntity->velocity->x = Physics::movementDelta;
				} else {
					mario->pEntity->velocity->x = 0.0f;
				}
			}
			mario->pEntity->velocity->y -= TerrainBlock::blockEdgeLength * 9.8f * elapsedTime;
			if (buttonPressed(GLFW_KEY_W) || buttonPressed(GLFW_KEY_S)) {
				if (canClimb) {
					mario->setState(Mario::CLIMBING);
					mario->pEntity->velocity->x = 0;
					mario->pEntity->velocity->y = 0;
				}
			}
		}
	} else if (mario->atGround) {
		if (groundBlock) {
			if (!x_touched_left_window && !x_touched_right_window) {
				TerrainBlock* touchedBlock = nullptr;
				if (touchedRightBlock) {
					touchedBlock = touchedRightBlock;
				} else if (touchedLeftBlock) {
					touchedBlock = touchedLeftBlock;
				}
				if (touchedBlock) {
					GLfloat yDelta = touchedBlock->position->y - groundBlock->position->y;
					if (yDelta <= ceil(TerrainBlock::blockEdgeLength / 7.0f)) {
						mario->position->y += yDelta;
					} else {
						if (touchedLeftBlock) {
							mario->position->x = touchedLeftBlock->position->x;
							eraseButton(GLFW_KEY_A);
						} else if (touchedRightBlock) {
							mario->position->x = touchedRightBlock->position->x - mario->width;
							eraseButton(GLFW_KEY_D);
						}
						mario->pEntity->velocity->x = 0.0f;
					}
				}
			} else {
				if (x_touched_left_window) {
					mario->position->x = 0.0f;
					mario->pEntity->velocity->x = 0.0f;
					eraseButton(GLFW_KEY_A);
				} else if (x_touched_right_window) {
					mario->pEntity->velocity->x = 0.0f;
					mario->position->x = Renderer::Window::WINDOW_WIDTH - mario->width;
					eraseButton(GLFW_KEY_D);
				}
				mario->pEntity->velocity->x = 0.0f;
			}
			if (buttonPressed(GLFW_KEY_A)) {
				if (!buttonPressed(GLFW_KEY_D)) {
					mario->pEntity->velocity->x = -Physics::movementDelta;
				} else {
					mario->pEntity->velocity->x = 0.0f;
				}
			} else if (!buttonPressed(GLFW_KEY_A)) {
				if (buttonPressed(GLFW_KEY_D)) {
					mario->pEntity->velocity->x = Physics::movementDelta;
				} else {
					mario->pEntity->velocity->x = 0.0f;
				}
			}
			if (buttonPressed(GLFW_KEY_SPACE)) {
				mario->pEntity->velocity->y = 1.5 * Physics::movementDelta;
			}
			if (buttonPressed(GLFW_KEY_W) || buttonPressed(GLFW_KEY_S)) {
				if (canClimb) {
					mario->setState(Mario::CLIMBING);
					mario->pEntity->velocity->x = 0;
					mario->pEntity->velocity->y = 0;
				}
			}
		} else {
			mario->setState(Mario::JUMPING);
		}
	} else if (mario->climbing) {
		if (!canClimb) {
			if (groundBlock) {
				mario->setState(Mario::AT_GROUND);
			} else {
				mario->setState(Mario::JUMPING);
			}
		} else {
			if (groundBlock) {
				mario->setState(Mario::AT_GROUND);
			}
			if (buttonPressed(GLFW_KEY_W)) {
				if (!buttonPressed(GLFW_KEY_S)) {
					mario->pEntity->velocity->y = Physics::movementDelta;
				} else {
					mario->pEntity->velocity->y = 0.0f;
				}
			} else if (!buttonPressed(GLFW_KEY_W)) {
				if (buttonPressed(GLFW_KEY_S)) {
					mario->pEntity->velocity->y = -Physics::movementDelta;
				} else {
					mario->pEntity->velocity->y = 0.0f;
				}
			}
		}
		if (buttonPressed(GLFW_KEY_A)) {
			if (!buttonPressed(GLFW_KEY_D)) {
				mario->pEntity->velocity->x = -Physics::movementDelta / 2.0f;
			} else {
				mario->pEntity->velocity->x = 0.0f;
			}
		} else if (!buttonPressed(GLFW_KEY_A)) {
			if (buttonPressed(GLFW_KEY_D)) {
				mario->pEntity->velocity->x = Physics::movementDelta / 2.0f;
			} else {
				mario->pEntity->velocity->x = 0.0f;
			}
		}
	}


	lastTime = glfwGetTime();
}

Collision PhysEngine::checkMarioCollision(Engine::Mario* mario, Engine::TerrainBlock* terrainBlock, bool exact) {
	// bool x = (mario->position->x >= terrainBlock->position->x + 1.0f)

	bool x = (mario->position->x <= terrainBlock->position->x + terrainBlock->width && mario->position->x + mario->width >= terrainBlock->position->x);
	bool y = mario->position->y + mario->height >= terrainBlock->position->y && mario->position->y <= terrainBlock->position->y + terrainBlock->height;

	return Collision(x && y, vectorDirection(*mario->pEntity->velocity), terrainBlock, Math::vec2<GLfloat>(*mario->pEntity->velocity));
}

Collision PhysEngine::checkMarioTouch(Engine::Mario* mario, Engine::TerrainBlock* terrainBlock) {
	// Math::vec2<GLfloat> upDownCompass[] = {
	// 	Math::vec2<GLfloat>(0.0f, 1.0f),	// up
	// 	Math::vec2<GLfloat>(0.0f, -1.0f)	// down
	// };


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
