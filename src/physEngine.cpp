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

	if (mario->position->x >= 220.f && mario->position->x + mario->width <= 280.0f &&
		mario->position->y >= 520.f && mario->position->y + mario->height <= 550.0f) {
			currentPhysLevel->level->playGameStateCallbacks->finishLevel();
		}

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
	if (mario->jumping) {
		if (groundBlock) {
			mario->setState(AnimatedActor::AT_GROUND);
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
					mario->setState(AnimatedActor::CLIMBING);
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
					mario->setState(AnimatedActor::CLIMBING);
					mario->pEntity->velocity->x = 0;
					mario->pEntity->velocity->y = 0;
				}
			}
		} else {
			mario->setState(AnimatedActor::JUMPING);
		}
	} else if (mario->climbing) {
		if (!canClimb) {
			if (groundBlock) {
				mario->setState(AnimatedActor::AT_GROUND);
			} else {
				mario->setState(AnimatedActor::JUMPING);
			}
		} else {
			if (groundBlock) {
				mario->setState(AnimatedActor::AT_GROUND);
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


	for (Barrel* barrel : currentPhysLevel->level->barrels) {
		barrel->position->x += barrel->pEntity->velocity->x * elapsedTime;
		barrel->position->y += barrel->pEntity->velocity->y * elapsedTime;

		bool x_touched_left_window = barrel->position->x <= 0.0f && barrel->position->x >= -collisionBias;
		bool x_touched_right_window = barrel->position->x + barrel->width <= Renderer::Window::WINDOW_WIDTH + collisionBias && barrel->position->x + barrel->width >= Renderer::Window::WINDOW_WIDTH;

		if ((mario->position->y > barrel->position->y + barrel->height) && mario->jumping && (mario->position->y - barrel->position->y - barrel->height) < TerrainBlock::blockEdgeLength * 2.0f && (mario->position->y - barrel->position->y - barrel->height) > 0  &&
		(lastTime - barrel->scoreTimer) >= 5.0f && mario->position->x <= barrel->position->x + barrel->width && mario->position->x + mario->width >= barrel->position->x) {
			barrel->scoreTimer = lastTime;
			currentPhysLevel->level->playGameStateCallbacks->addPoints(100);
		}

		if (mario->position->x <= barrel->position->x + barrel->width && mario->position->x + mario->width >= barrel->position->x &&
		mario->position->y <= barrel->position->y + barrel->height && mario->position->y + mario->height >= barrel->position->y) {
			currentPhysLevel->level->playGameStateCallbacks->decreaseLife();
		}
		// std::vector<Collision> marioCollisions;
		// bool hasGroundBeneath = false;

		TerrainBlock* groundBlock = nullptr;

		srand((int)(glfwGetTime() * 1389));

		for (TerrainBlock* terrainBlock: currentPhysLevel->level->terrainBlocks) {
			if (terrainBlock->type == Engine::TerrainBalk) {
				bool x_collided = (barrel->position->x <= terrainBlock->position->x + terrainBlock->width && barrel->position->x + barrel->width >= terrainBlock->position->x);
				bool y_touched_up = barrel->position->y <= terrainBlock->position->y + terrainBlock->height && barrel->position->y >= terrainBlock->position->y + terrainBlock->height - collisionBias;
				if (y_touched_up && x_collided) {
					groundBlock = terrainBlock;
				}
			}
		}

		Ladder* climbLadder = nullptr;
		for (Ladder* ladder : currentPhysLevel->level->ladders) {
			bool x_collided = barrel->position->x + barrel->width * 3 / 4.0f <= ladder->location.x + ladder->dimensions.x && barrel->position->x + barrel->width / 4.0f >= ladder->location.x;
			bool y_collided = barrel->position->y >= ladder->location.y && barrel->position->y + barrel->height <= ladder->location.y + ladder->dimensions.y;
			if (x_collided && y_collided && (barrel->position->y - ladder->location.y) > 20.0f) {
				climbLadder = ladder;
			}
		}

		if (barrel->jumping) {
			barrel->pEntity->velocity->y -= TerrainBlock::blockEdgeLength * 9.8f * elapsedTime;
			if (groundBlock) {
				barrel->pEntity->velocity->y = 0.0f;
				barrel->setState(AnimatedActor::AT_GROUND);
				barrel->position->y = groundBlock->position->y + groundBlock->height;
			} else {
				if (x_touched_left_window) {
					barrel->position->x = 0.0f;
					barrel->pEntity->velocity->x = -barrel->pEntity->velocity->x;
				} else if (x_touched_right_window) {
					barrel->position->x = Renderer::Window::WINDOW_WIDTH - barrel->width;
					barrel->pEntity->velocity->x = -barrel->pEntity->velocity->x;
				}
				if (climbLadder) {
					if (rand() % 40 == 0) {
						barrel->setState(AnimatedActor::CLIMBING);
						barrel->pEntity->velocity->x = 0.0f;
						if (barrel->face == AnimatedActor::FACING_RIGHT) {
							barrel->face = AnimatedActor::FACING_LEFT;
						} else  {
							barrel->face = AnimatedActor::FACING_RIGHT;
						}
					}
				}
			}
		} else if (barrel->atGround) {
			if (!groundBlock) {
				barrel->setState(AnimatedActor::JUMPING);
			} else {
				if (x_touched_left_window) {
					barrel->position->x = 0.0f;
					barrel->pEntity->velocity->x = -barrel->pEntity->velocity->x;
				} else if (x_touched_right_window) {
					barrel->position->x = Renderer::Window::WINDOW_WIDTH - barrel->width;
					barrel->pEntity->velocity->x = -barrel->pEntity->velocity->x;
				}
				if (climbLadder) {
					if (rand() % 40 == 0) {
						barrel->setState(AnimatedActor::CLIMBING);
						barrel->pEntity->velocity->x = 0.0f;
						if (barrel->face == AnimatedActor::FACING_RIGHT) {
							barrel->face = AnimatedActor::FACING_LEFT;
						} else  {
							barrel->face = AnimatedActor::FACING_RIGHT;
						}
					}
				}
			}
		} else if (barrel->climbing) {
			if ((!climbLadder)) {
				if (groundBlock) {
					barrel->setState(AnimatedActor::AT_GROUND);
					barrel->pEntity->velocity->y = 0.0f;
					if (barrel->face == AnimatedActor::FACING_RIGHT) {
						barrel->pEntity->velocity->x = Physics::movementDelta * 1.5f;
					} else {
						barrel->pEntity->velocity->x = -Physics::movementDelta * 1.5f;
					}
				} else {
					barrel->pEntity->velocity->y = -Physics::movementDelta;
				}
			} else if (groundBlock && (barrel->position->y - climbLadder->location.y) <= ceil(TerrainBlock::blockEdgeLength / 7.0f)) {
				barrel->setState(AnimatedActor::AT_GROUND);
				barrel->pEntity->velocity->y = 0.0f;
				if (barrel->face == AnimatedActor::FACING_RIGHT) {
					barrel->pEntity->velocity->x = Physics::movementDelta * 1.5f;
				} else {
					barrel->pEntity->velocity->x = -Physics::movementDelta * 1.5f;
				}
			} else {
				barrel->pEntity->velocity->y = -Physics::movementDelta;
			}
		}
		if (barrel->position->y < 0) {
			currentPhysLevel->level->barrels.erase(std::find(currentPhysLevel->level->barrels.begin(), currentPhysLevel->level->barrels.end(), barrel));
		}

	}

	lastTime = glfwGetTime();
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
