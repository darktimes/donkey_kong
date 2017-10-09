#include <algorithm>
#include "gameStates.hpp"
#include "gameLevel.hpp"
#include "game.hpp"
#include "utils.hpp"

using namespace Rendering;
using namespace Utils;

const std::string MenuGameState::TAG = "MenuGameState";
const std::string PlayGameState::TAG = "PlayGameState";

/**
* Defines a common cause to exit the state.
*/
bool GameState::exits() {
	return glfwWindowShouldClose(
		Renderer::getInstance()->getWindow()->windowHandle);
}

MenuGameState::MenuGameState(): GameState() {
	viewList.push_back(std::unique_ptr<TextView>(
		new TextView(std::string("To start a game press ENTER"),
		 Math::vec2f(50.0f, 650.0f), 0.7, Math::vec3f(1.0f, 0.0f, 0.0f))
	));

	viewList.push_back(std::unique_ptr<TextView>(
		new TextView(std::string("To exit press ESC"),
		 Math::vec2f(150.0f, 550.0f), 0.7, Math::vec3f(1.0f, 0.0f, 0.0f))
	));

	viewList.push_back(std::unique_ptr<TextView>(
		new TextView(std::string("Records"),
		 Math::vec2f(220.0f, 400.0f), 0.7, Math::vec3f(0.8f, 0.0f, 0.0f))
	));

	GLfloat recordYpos = 370.0f;
	for (unsigned k = 0; k < RecordTable::RECORD_COUNT; k++) {

		const std::unique_ptr<Record>& currentRecord = Game::getInstance().recordTable.getRecords()[k];

		viewList.push_back(std::unique_ptr<TextView>(
			new TextView(std::to_string(k + 1) + ". ", Math::vec2f(150.0f, recordYpos - (k + 1) * 20.0f),
			 0.5, Math::vec3f(1.0f, 0.5f, 0.5f))
		));

		viewList.push_back(std::unique_ptr<TextView>(
			new TextView(currentRecord->playerName, Math::vec2f(175.0f, recordYpos - (k + 1) * 20.0f),
			 0.5, Math::vec3f(1.0f, 0.5f, 0.5f))
		));


		viewList.push_back(std::unique_ptr<TextView>(
			new TextView(std::to_string(currentRecord->score), Math::vec2f(350.0f, recordYpos - (k + 1) * 20.0f),
			 0.5, Math::vec3f(1.0f, 0.5f, 0.5f))
		));

	}
}

/**
* Some controller-/model stuff, that might be present in the future
*/
void MenuGameState::stateTick() {
  //nothing
}

void MenuGameState::processInput(GLFWwindow* window, int key, int scancode,
	 int action, int mode) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		Game::getInstance().changeState(new PlayGameState());
}

std::string MenuGameState::getTag() {
	return TAG;
}

PlayGameState::PlayGameState() : GameState() {
	changeSubState(new ActivePlayGameSubState(this));
}

void PlayGameState::changeSubState(PlayGameSubState* newState) {
	subState.reset(newState);
}

void PlayGameState::stateTick() {
	subState->stateTick();
}

PlayGameState::~PlayGameState() {

}

std::string PlayGameState::getTag() {
	return TAG;
}

void PlayGameState::processInput(GLFWwindow* window, int key, int scancode,
	 int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
	 Game::getInstance().changeState(new MenuGameState());
 } else {
	 subState->processInput(window, key, scancode, action, mode);
 }
}

PlayGameSubState::PlayGameSubState(PlayGameState* parent) {
	this->parent = parent;
}


//-----------------------------------------------------------------------------
// Active
//-----------------------------------------------------------------------------
const float ActivePlayGameSubState::collisionBias = 2.0f;
const float ActivePlayGameSubState::fallingConstant = -98.0f;
const float ActivePlayGameSubState::movementDelta = 90.0f;
const float ActivePlayGameSubState::barrelMovmentDelta = 90.0f * 1.5f;
const float ActivePlayGameSubState::climbingDelta = 45.0f;
const float ActivePlayGameSubState::barrelClimbingDelta = 45.0f * 1.5f;

ActivePlayGameSubState::ActivePlayGameSubState(PlayGameState* parent):
 PlayGameSubState(parent) {
	srand((int)(Utils::getTime() * 1389));

	gameSession = std::unique_ptr<GameSession>(new GameSession());
	setLevel(new LevelOne());

	const std::unique_ptr<Record>& firstRecord =
	 Game::getInstance().recordTable.getRecords()[0];

	viewList.push_back(std::unique_ptr<TextView>(
		new TextView(std::string("Score"),
		 Math::vec2f(0.0f, 780.0f), 0.5f, Math::vec3f(0.5f, 0.0f, 0.0f))
	));

	viewList.push_back(std::unique_ptr<TextView>(
		new TextView(std::string("Max.Record"),
		 Math::vec2f(200.0f, 780.0f), 0.5f, Math::vec3f(0.5f, 0.0f, 0.0f))
	));

	viewList.push_back(std::unique_ptr<TextView>(
		new TextView(std::string("Bonus"),
		 Math::vec2f(450.0f, 780.0f), 0.5f, Math::vec3f(0.5f, 0.0f, 0.0f))
	));

	viewList.push_back(std::unique_ptr<TextView>(
		new TextView(std::string((firstRecord->playerName) + ": " + std::to_string(firstRecord->score)),
		 Math::vec2f(200.0f, 760.0f), 0.5f, Math::vec3f(0.5f, 0.5f, 0.0f))
	));

	scoreText = std::unique_ptr<TextView>(
		new TextView(std::to_string(gameSession->points),
		 Math::vec2f(0.0f, 760.0f), 0.5f, Math::vec3f(0.0f, 0.0f, 0.5f))
	);

	currentBonusText = std::unique_ptr<TextView>(
		new TextView(std::to_string(gameSession->currentBonus),
		 Math::vec2f(450.0f, 760.0f), 0.5f, Math::vec3f(0.0f, 0.0f, 0.5f))
	);

	lifeText = std::unique_ptr<TextView>(
		new TextView(std::to_string(gameSession->lifeCount),
		 Math::vec2f(450.0f, 720.0f), 0.5f, Math::vec3f(0.0f, 0.0f, 0.5f))
	);

	fpsText = std::unique_ptr<TextView>(
		new TextView(std::to_string(0),
		 Math::vec2f(0.0f, 720.0f), 0.5f, Math::vec3f(0.0f, 0.0f, 0.5f))
	);

	viewList.push_back(std::unique_ptr<TextView>(
		new TextView(std::string("Lifes"),
		 Math::vec2f(450.0f, 740.0f), 0.5f, Math::vec3f(0.5f, 0.0f, 0.0f))
	));

	viewList.push_back(std::unique_ptr<TextView>(
		new TextView(std::string("FPS"),
		 Math::vec2f(0.0f, 740.0f), 0.5f, Math::vec3f(0.5f, 0.0f, 0.0f))
	));

	viewList.push_back(std::unique_ptr<TextView>(
		new TextView(std::string("Finish"),
		 Math::vec2f(235.0f, 525.0f), 0.5f, Math::vec3f(0.0f, 1.0f, 0.0f))
	));

	fpsDisplayCounter = 0;
	fpsCounter = 0;
	fpsTimer = 0;
	bonusTimer = 0;
	moveTimer = 0;
}

void ActivePlayGameSubState::setLevel(Level* level) {
	currentLevel.reset(level);
	gameSession->setLevel(level);
}

void ActivePlayGameSubState::processInput(GLFWwindow* window, int key,
 int scancode, int action, int mode) {
	 Mario* mario = currentLevel->mario.get();
	 if (key == GLFW_KEY_D) {
		 if (action == GLFW_PRESS) {
			 directActorRight(mario);
		 } else if (action == GLFW_RELEASE) {
			 stopActorRight(mario);
		 }
	 } else if (key == GLFW_KEY_A) {
		 if (action == GLFW_PRESS) {
			 directActorLeft(mario);
		 } else if (action == GLFW_RELEASE) {
			 stopActorLeft(mario);
		 }
	 } else if (key == GLFW_KEY_S) {
		 if (action == GLFW_PRESS) {
			 directActorDown(mario);

		 } else if (action == GLFW_RELEASE) {
			 stopActorDown(mario);
		 }
	 } else if (key == GLFW_KEY_W && currentLevel->canClimb(mario)) {
		 if (action == GLFW_PRESS) {
			directActorUp(mario);
		 } else if (action == GLFW_RELEASE) {
			 stopActorUp(mario);
		 }
	 } else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		  jumpActor(mario);

	 }
}
void ActivePlayGameSubState::directActorLeft(Actor* actor) {
	actor->faceDirection = Actor::FACE_LEFT;
 if (actor->climbing) {
 	actor->physEntity->velocity.x = -movementDelta / 4.0f;
 } else {
 	actor->physEntity->velocity.x = -movementDelta;
 }
}

void ActivePlayGameSubState::directActorRight(Actor* actor) {
	actor->faceDirection = Actor::FACE_RIGHT;
  if (actor->climbing) {
 	 actor->physEntity->velocity.x = movementDelta / 4.0f;
  } else {
 	 actor->physEntity->velocity.x = movementDelta;
  }
}

void ActivePlayGameSubState::directActorUp(Actor* actor) {
	actor->climbing = true;
	if (actor->physEntity->velocity.x != 0) {
		if (actor->faceDirection == Actor::FACE_LEFT) {
			actor->physEntity->velocity.x = -movementDelta / 4.0f;
		} else {
			actor->physEntity->velocity.x = +movementDelta / 4.0f;
		}
	}
	actor->physEntity->acceleration.y = 0;
	actor->physEntity->velocity.y = +climbingDelta;
}

void ActivePlayGameSubState::directActorDown(Actor* actor) {
	if (currentLevel->canClimb(actor)) {
		if (actor->physEntity->velocity.x != 0) {
		 if (actor->faceDirection == Actor::FACE_LEFT) {
			 actor->physEntity->velocity.x = -movementDelta / 4.0f;
		 } else {
			 actor->physEntity->velocity.x = +movementDelta / 4.0f;
		 }
		}
		actor->physEntity->velocity.y = -climbingDelta;
		actor->climbing = true;
		actor->physEntity->acceleration.y = 0;
	}
}

void ActivePlayGameSubState::jumpActor(Actor* actor) {
	if (!actor->jumping()) {
		if (actor->climbing) {
			actor->climbing = false;
		} else {
			actor->physEntity->velocity.y = 95.0f;
			actor->physEntity->acceleration.y = fallingConstant;
		}
	}
}

void ActivePlayGameSubState::stopActorLeft(Actor* actor) {
	if (actor->faceDirection == Actor::FACE_LEFT) {
		actor->physEntity->velocity.x = 0;
	}
}
void ActivePlayGameSubState::stopActorRight(Actor* actor) {
	if (actor->faceDirection == Actor::FACE_RIGHT) {
		actor->physEntity->velocity.x = 0;
	}
}

void ActivePlayGameSubState::stopActorUp(Actor* actor) {
	if (actor->climbing && actor->physEntity->velocity.y > 0 )
	 actor->physEntity->velocity.y = 0;
}
void ActivePlayGameSubState::stopActorDown(Actor* actor) {
	if (actor->climbing && actor->physEntity->velocity.y < 0 )
		actor->physEntity->velocity.y = 0;
}

std::string ActivePlayGameSubState::getTag() {
	return std::string("ActivePlayGameSubState");
}

void ActivePlayGameSubState::stateTick() {
	double currentTime = Utils::getTime();
	fpsCounter++;
	if ((currentTime - fpsTimer) >= 1.0f) {
		fpsTimer = currentTime;
		fpsDisplayCounter = fpsCounter;
		fpsCounter = 0;
	}

	if (currentTime - bonusTimer > 1.0f) {
		bonusTimer = currentTime;
		if (gameSession->currentBonus != 0.0f) {
			gameSession->currentBonus -= 10.0f;
		}
	}

	if (currentTime - currentLevel->mario->temporalImmunityTimer > 2.5) {
		currentLevel->mario->temporalImmunity = false;
	}

	scoreText->text = std::to_string(gameSession->points);
	lifeText->text = std::to_string(gameSession->lifeCount);
	currentBonusText->text = std::to_string(gameSession->currentBonus);
	fpsText->text = std::to_string(fpsDisplayCounter);

	//finish game
	if (currentLevel->mario->position.x >= currentLevel->finishAreaPosition.x
		 && currentLevel->mario->position.x + currentLevel->mario->dimensions.x <= currentLevel->finishAreaPosition.x + currentLevel->finishAreaDimensions.x
		 && currentLevel->mario->position.y >= currentLevel->finishAreaPosition.y
		 && currentLevel->mario->position.y + currentLevel->mario->dimensions.y <= currentLevel->finishAreaPosition.y + currentLevel->finishAreaDimensions.y) {
		int score = gameSession->points + gameSession->currentBonus;
		if (Game::getInstance().recordTable.isNewScore(score)) {
			parent->changeSubState(new OverRecordPlayGameSubState(parent, score));
		} else {
			parent->changeSubState(new OverPlayGameSubState(parent, score));
		}
		return;
	}

	moveMario();

	//process collisions
	for (const auto& barrel: currentLevel->barrels) {
		if (currentLevel->mario->collides(barrel.get()) && !currentLevel->mario->temporalImmunity) {
			if (gameSession->lifeCount > 1) {
				gameSession->lifeCount--;
				currentLevel->mario->temporalImmunity = true;
				currentLevel->mario->temporalImmunityTimer = currentTime;
			} else {
				parent->changeSubState(new OverPlayGameSubState(parent, gameSession->points + gameSession->currentBonus));
				return;
			}
		} else if (currentLevel->mario->over(barrel.get()) && currentTime - barrel->scoreTimer >= 5.0f) {
			barrel->scoreTimer = currentTime;
			gameSession->points += 100;
		}
		moveBarrel(barrel.get());
	}

	currentLevel->barrels.erase(std::remove_if(currentLevel->barrels.begin(),
	currentLevel->barrels.end(), [](const auto& barrel) { return barrel->position.y < 0; }),
	currentLevel->barrels.end());
	currentLevel->tick();
	moveTimer = currentTime;

}

void ActivePlayGameSubState::moveMario() {
	Mario* mario = currentLevel->mario.get();

	double elapsed = Utils::getTime() - moveTimer;

	mario->position.x += mario->physEntity->velocity.x * elapsed;
	mario->position.y += mario->physEntity->velocity.y * elapsed;

	bool x_touched_left_window = mario->position.x <= 0.0f && mario->position.x >= -collisionBias;
	bool x_touched_right_window = mario->position.x + mario->dimensions.x <= Window::WINDOW_WIDTH + collisionBias && mario->position.x + mario->dimensions.x >= Window::WINDOW_WIDTH;

	// std::vector<Collision> marioCollisions;
	// bool hasGroundBeneath = false;

	TerrainBlock* groundBlock = nullptr;
	TerrainBlock* touchedRightBlock = nullptr;
	TerrainBlock* touchedLeftBlock = nullptr;

	for (const auto& terrainBlock: currentLevel->terrainBlocks) {
		bool x_collided = (mario->position.x <= terrainBlock->position.x + terrainBlock->dimensions.x && mario->position.x + mario->dimensions.x >= terrainBlock->position.x);
		bool y_collided = mario->position.y + mario->dimensions.y >= terrainBlock->position.y && mario->position.y <= terrainBlock->position.y + terrainBlock->dimensions.y;

		bool x_touched_right = mario->position.x + mario->dimensions.x >= terrainBlock->position.x && mario->position.x + mario->dimensions.x <= terrainBlock->position.x + collisionBias;
		bool x_touched_left = mario->position.x >= terrainBlock->position.x + terrainBlock->dimensions.x - collisionBias && mario->position.x <= terrainBlock->position.x + terrainBlock->dimensions.x;
		bool y_touched_up = mario->position.y <= terrainBlock->position.y + terrainBlock->dimensions.y && mario->position.y >= terrainBlock->position.y + terrainBlock->dimensions.y - collisionBias;

		if (y_touched_up && x_collided) {
			groundBlock = terrainBlock.get();
		}
		if (y_collided && x_touched_left) {
			touchedLeftBlock = terrainBlock.get();
		} else if (y_collided && x_touched_right) {
			touchedRightBlock = terrainBlock.get();
		}

	}

	bool canClimb = currentLevel->canClimb(mario);

	if (mario->jumping()) {
		if (groundBlock) {
			mario->physEntity->velocity.y = 0.0f;
			mario->position.y = groundBlock->position.y + groundBlock->dimensions.y;
			mario->physEntity->acceleration.y = 0.0f;
		} else {
			if (x_touched_left_window) {
				mario->position.x = 0.0f;
				mario->physEntity->velocity.x = 0.0f;
			} else if (x_touched_right_window) {
				mario->position.x = Window::WINDOW_WIDTH - mario->dimensions.x;
				mario->physEntity->velocity.x = 0.0f;
			}
			mario->physEntity->velocity.y += mario->physEntity->acceleration.y * elapsed;
		}
	} else if (mario->atGround()) {
		if (groundBlock) {
			if (!x_touched_left_window && !x_touched_right_window) {
				if (mario->physEntity->velocity.x != 0) {
					if (mario->faceDirection == Actor::FACE_LEFT) {
						mario->physEntity->velocity.x = -movementDelta;
					} else {
						mario->physEntity->velocity.x = movementDelta;
					}
				}
				TerrainBlock* touchedBlock = nullptr;
				if (touchedRightBlock) {
					touchedBlock = touchedRightBlock;
				} else if (touchedLeftBlock) {
					touchedBlock = touchedLeftBlock;
				}
				if (touchedBlock) {
					GLfloat yDelta = touchedBlock->position.y - groundBlock->position.y;
					if (yDelta <= ceil(GameObject::blockEdgeLength / 7.0f)) {
						mario->position.y += yDelta;
					} else {
						if (touchedLeftBlock) {
							mario->position.x = touchedLeftBlock->position.x;
						} else if (touchedRightBlock) {
							mario->position.x = touchedRightBlock->position.x - mario->dimensions.x;
						}
						mario->physEntity->velocity.x = 0.0f;
					}
				}
			} else {
				if (x_touched_left_window) {
					mario->position.x = 0.0f;
					mario->physEntity->velocity.x = 0.0f;
				} else if (x_touched_right_window) {
					mario->physEntity->velocity.x = 0.0f;
					mario->position.x = Window::WINDOW_WIDTH - mario->dimensions.x;
				}
				mario->physEntity->velocity.x = 0.0f;
			}
		} else {
			mario->physEntity->acceleration.y = fallingConstant;
		}
	} else if (mario->climbing) {
		if (!canClimb) {
			if (groundBlock) {
				mario->physEntity->acceleration.y = 0.0f;
				mario->physEntity->velocity.y = 0.0f;
			} else {
				mario->physEntity->acceleration.y = fallingConstant;
			}
			mario->climbing = false;
		} else {
			if (groundBlock) {
				// mario->climbing = false;
				mario->physEntity->acceleration.y = 0.0f;
				mario->physEntity->velocity.y = 0.0f;
			}
		}
	}


}

void ActivePlayGameSubState::moveBarrel(Barrel* barrel) {
	double elapsedTime = Utils::getTime() - moveTimer;

	barrel->position.x += barrel->physEntity->velocity.x * elapsedTime;
	barrel->position.y += barrel->physEntity->velocity.y * elapsedTime;

	bool x_touched_left_window = barrel->position.x <= 0.0f && barrel->position.x >= -collisionBias;
	bool x_touched_right_window = barrel->position.x + barrel->dimensions.x <= Window::WINDOW_WIDTH + collisionBias && barrel->position.x + barrel->dimensions.x >= Window::WINDOW_WIDTH;

	TerrainBlock* groundBlock = nullptr;

	for (const auto& terrainBlock: currentLevel->terrainBlocks) {

		bool x_collided = (barrel->position.x <= terrainBlock->position.x + terrainBlock->dimensions.x && barrel->position.x + barrel->dimensions.x >= terrainBlock->position.x);
		bool y_touched_up = barrel->position.y <= terrainBlock->position.y + terrainBlock->dimensions.y && barrel->position.y >= terrainBlock->position.y + terrainBlock->dimensions.y - collisionBias;
		if (y_touched_up && x_collided) {
			groundBlock = terrainBlock.get();
		}
	}

	TerrainBlock* climbLadder = nullptr;
	for (const auto& ladder : currentLevel->ladders) {
		bool x_collided = barrel->position.x + barrel->dimensions.x * 3 / 4.0f <= ladder->position.x + ladder->dimensions.x && barrel->position.x + barrel->dimensions.x / 4.0f >= ladder->position.x;
		bool y_collided = barrel->position.y >= ladder->position.y && barrel->position.y + barrel->dimensions.y <= ladder->position.y + ladder->dimensions.y;
		if (x_collided && y_collided && (barrel->position.y - ladder->position.y) > 20.0f) {
			climbLadder = ladder.get();
		}
	}

	if (barrel->jumping()) {
		barrel->physEntity->velocity.y += fallingConstant * elapsedTime;
		if (groundBlock) {
			if (barrel->faceDirection == Actor::FACE_RIGHT) {
				barrel->physEntity->velocity.x = barrelMovmentDelta;
			} else {
				barrel->physEntity->velocity.x = -barrelMovmentDelta;
			}
			barrel->physEntity->velocity.y = 0.0f;
			barrel->physEntity->acceleration.y = 0.0f;
			barrel->position.y = groundBlock->position.y + groundBlock->dimensions.y;
		} else {
			if (x_touched_left_window) {
				barrel->position.x = 0.0f;
				barrel->faceDirection = Actor::FACE_RIGHT;
				barrel->physEntity->velocity.x = -barrel->physEntity->velocity.x;
			} else if (x_touched_right_window) {
				barrel->faceDirection = Actor::FACE_LEFT;
				barrel->position.x = Window::WINDOW_WIDTH - barrel->dimensions.x;
				barrel->physEntity->velocity.x = -barrel->physEntity->velocity.x;
			}
			if (climbLadder) {
				if (rand() % 40 == 0) {
					barrel->climbing = true;
					barrel->physEntity->acceleration.y = 0.0f;
					barrel->physEntity->velocity.y = -barrelClimbingDelta;
					barrel->physEntity->velocity.x = 0.0f;
					if (barrel->faceDirection == Actor::FACE_RIGHT) {
						barrel->faceDirection = Actor::FACE_LEFT;
					} else  {
						barrel->faceDirection = Actor::FACE_RIGHT;
					}
				}
			}
		}
	} else if (barrel->atGround()) {
		if (!groundBlock) {
			barrel->physEntity->acceleration.y = fallingConstant;
		} else {
			if (x_touched_left_window) {
				barrel->faceDirection = Actor::FACE_RIGHT;
				barrel->position.x = 0.0f;
				barrel->physEntity->velocity.x = -barrel->physEntity->velocity.x;
			} else if (x_touched_right_window) {
				barrel->faceDirection = Actor::FACE_LEFT;
				barrel->position.x = Window::WINDOW_WIDTH - barrel->dimensions.x;
				barrel->physEntity->velocity.x = -barrel->physEntity->velocity.x;
			}
			if (climbLadder) {
				if (rand() % 40 == 0) {
					barrel->climbing = true;
					barrel->physEntity->acceleration.y = 0.0f;
					barrel->physEntity->velocity.y = -barrelClimbingDelta;
					barrel->physEntity->velocity.x = 0.0f;
					if (barrel->faceDirection == Actor::FACE_RIGHT) {
						barrel->faceDirection = Actor::FACE_LEFT;
					} else  {
						barrel->faceDirection = Actor::FACE_RIGHT;
					}
				}
			}
		}
	} else if (barrel->climbing) {
		if ((!climbLadder)) {
			if (groundBlock) {
				barrel->physEntity->acceleration.y = 0.0f;
				barrel->physEntity->velocity.y = 0.0f;
				if (barrel->faceDirection == Actor::FACE_RIGHT) {
					barrel->physEntity->velocity.x = barrelMovmentDelta;
				} else {
					barrel->physEntity->velocity.x = -barrelMovmentDelta;
				}
			} else {
				barrel->physEntity->acceleration.y = fallingConstant;
			}
			barrel->climbing = false;
		} else if (groundBlock && (barrel->position.y - climbLadder->position.y) <= ceil(TerrainBlock::blockEdgeLength / 7.0f)) {
			barrel->physEntity->acceleration.y = 0.0f;
			barrel->physEntity->velocity.y = 0.0f;
			if (barrel->faceDirection == Actor::FACE_RIGHT) {
				barrel->physEntity->velocity.x = barrelMovmentDelta;
			} else {
				barrel->physEntity->velocity.x = -barrelMovmentDelta;
			}
		}
	}
}

void ActivePlayGameSubState::processCurrentLevel() {

}


ActivePlayGameSubState::~ActivePlayGameSubState() {

}
//-----------------------------------------------------------------------------
// Over
//-----------------------------------------------------------------------------

OverPlayGameSubState::OverPlayGameSubState(PlayGameState* parent, int score):
 PlayGameSubState(parent) {
	viewList.push_back(std::unique_ptr<TextView>(
	 new TextView(std::string("GAME OVER"),
		Math::vec2f(190.0f, 650.0f), 0.7f, Math::vec3f(1.0f, 0.0f, 0.0f))
	));

	viewList.push_back(std::unique_ptr<TextView>(
	 new TextView(std::string("SCORE:" + std::to_string(score)),
		Math::vec2f(190.0f, 550.0f), 0.7f, Math::vec3f(1.0f, 0.0f, 0.0f))
	));

	viewList.push_back(std::unique_ptr<TextView>(
	 new TextView(std::string("To exit press ESC"),
		Math::vec2f(150.0f, 450.0f), 0.7f, Math::vec3f(1.0f, 0.0f, 0.0f))
	));

}

void OverPlayGameSubState::processInput(GLFWwindow* window, int key,
 int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
	 Game::getInstance().changeState(new MenuGameState());
	}
}

std::string OverPlayGameSubState::getTag() {
	return std::string("ActivePlayGameSubState");
}

void OverPlayGameSubState::stateTick() {
	//nothing
}

//-----------------------------------------------------------------------------
// Over with Record
//-----------------------------------------------------------------------------

OverRecordPlayGameSubState::OverRecordPlayGameSubState(PlayGameState* parent, int score):
 PlayGameSubState(parent), score(score) {

 viewList.push_back(std::unique_ptr<TextView>(
 	 new TextView(std::string("NEW RECORD"),
 		Math::vec2f(190.0f, 650.0f), 0.7f, Math::vec3f(1.0f, 0.0f, 0.0f))
 ));

 viewList.push_back(std::unique_ptr<TextView>(
 	 new TextView(std::string("SCORE:") + std::to_string(score),
 		Math::vec2f(190.0f, 610.0f), 0.7f, Math::vec3f(1.0f, 0.0f, 0.0f))
 ));

 viewList.push_back(std::unique_ptr<TextView>(
 	 new TextView(std::string("YOUR NAME"),
 		Math::vec2f(190.0f, 450.0f), 0.7f, Math::vec3f(1.0f, 0.0f, 0.0f))
 ));

 recordHolderNameText = std::unique_ptr<TextView>(
 	 new TextView(recordHolderName,
 		Math::vec2f(190.0f, 410.0f), 0.7f, Math::vec3f(1.0f, 0.0f, 0.0f))
 );

}

void OverRecordPlayGameSubState::processInput(GLFWwindow* window, int key,
 int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		if (recordHolderName.size() <= 8 && key != GLFW_KEY_ENTER) {
			if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
				recordHolderName += (char) (key - GLFW_KEY_A + 65);
			}
		} else {
			Game::getInstance().recordTable.addRecord(new Record(score, recordHolderName));
			Game::getInstance().changeState(new MenuGameState());
		}
	}
}

std::string OverRecordPlayGameSubState::getTag() {
	return std::string("ActivePlayGameSubState");
}

void OverRecordPlayGameSubState::stateTick() {
	recordHolderNameText->text = recordHolderName;
}
