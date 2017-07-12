#include "game.hpp"
#include "gameStates.hpp"
#include "resourceManager.hpp"
#include "math.hpp"
#include "utils.hpp"
#include "gameLevel.hpp"
#include "renderer.hpp"
#include "records.hpp"
//#include "records.hpp"
#include <cmath>

using namespace Engine;
using namespace Utils;
using namespace Physics;

const std::string MenuGameState::TAG = "MenuGameState";

const std::string PlayGameState::TAG = "PlayGameState";


GameState::GameState() {

}

bool GameState::exits() {
	return glfwWindowShouldClose(Game::getGameWindow()->windowHandle);
}

void GameState::tick() {
	if (!exits()) {
		stateTick();
	}
}

MenuGameState::MenuGameState() : GameState() {

}


void MenuGameState::stateTick() {
	Renderer::SpriteRenderer::renderText("To start a game press ENTER", 50.0f, 650.0f, 0.7, Math::vec3<GLfloat>(1.0f, 0.0f, 0.0f));
	Renderer::SpriteRenderer::renderText("To exit press ESC", 150.0f, 550.0f, 0.7, Math::vec3<GLfloat>(1.0f, 0.0f, 0.0f));
	GLfloat recordYpos = 400.0f;
	Renderer::SpriteRenderer::renderText("Records", 220.0f, recordYpos, 0.7, Math::vec3<GLfloat>(0.8f, 0.0f, 0.0f));
	recordYpos-=30.0f;
	for (unsigned k = 0; k < RecordTable::RECORD_COUNT; k++) {
		Record* currentRecord = ResourceManager::recordTable->getRecords()[k];
		Renderer::SpriteRenderer::renderText(std::to_string(k + 1) + ". " , 150.0f, recordYpos - (k + 1) * 20.0f, 0.5, Math::vec3<GLfloat>(1.0f, 0.5f, 0.5f));
		Renderer::SpriteRenderer::renderText(currentRecord->playerName, 175.0f, recordYpos - (k + 1) * 20.0f, 0.5, Math::vec3<GLfloat>(1.0f, 0.5f, 0.5f));
		Renderer::SpriteRenderer::renderText(std::to_string(currentRecord->score) , 350.0f, recordYpos - (k + 1) * 20.0f, 0.5, Math::vec3<GLfloat>(1.0f, 0.5f, 0.5f));
	//+ currentRecord->playerName + " " + std::to_string(currentRecord->score)
	}
}

bool MenuGameState::exits() {
	return GameState::exits();
}

std::string MenuGameState::getTag() {
	return TAG;
}

void MenuGameState::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    		glfwSetWindowShouldClose(window, GL_TRUE);
	} else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		Game::changeState(new PlayGameState());
	}
}


PlayGameState::PlayGameState() : GameState() {
	gameSession = new GameSession();
	gameSession->setLevel(new Level1(this));
	playState = ACTIVE;
	prevSecondStamp = glfwGetTime();
	PhysEngine::init(gameSession->currentLevel);
}

PlayGameState::~PlayGameState() {
	delete gameSession;
}

void PlayGameState::stateTick() {
	double currentTime = glfwGetTime();

	PhysEngine::tick();
	gameSession->currentLevel->mario->handleAnimationState();

	gameSession->currentLevel->draw();

	if (currentTime - prevSecondStamp > 1.0f) {
		prevSecondStamp += 1.0f;
		if (gameSession->currentBonus != 0.0f) {
			gameSession->currentBonus -= 10.0f;
		}
	}
	Record* firstRecord = ResourceManager::recordTable->getRecords()[0];
	Renderer::SpriteRenderer::renderText("Score", 0.0f, 780.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.0f,0.0f));
	Renderer::SpriteRenderer::renderText("Max.Record", 200.0f, 780.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.0f, 0.0f));
	Renderer::SpriteRenderer::renderText("Bonus", 450.0f, 780.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.0f, 0.0f));
	Renderer::SpriteRenderer::renderText(std::string((firstRecord->playerName) + ": " + std::to_string(firstRecord->score)), 200.0f, 760.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.5f, 0.0f));
	Renderer::SpriteRenderer::renderText(std::to_string(gameSession->points), 0.0f, 760.0f, 0.5f, Math::vec3<GLfloat>(0.0f, 0.0f, 0.5f));
	Renderer::SpriteRenderer::renderText(std::to_string(gameSession->currentBonus), 450.0f, 760.0f, 0.5f, Math::vec3<GLfloat>(0.0f, 0.0f, 0.5f));
}

bool PlayGameState::exits() {
	return GameState::exits();
}

std::string PlayGameState::getTag() {
	return TAG;
}

void PlayGameState::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) {


	if (playState == OVER) {

	} else {
		GLfloat velocityChangeValue = Physics::movementDelta;
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			Game::changeState(new MenuGameState());
		} else if (key == GLFW_KEY_D) {
			if (action == GLFW_PRESS) {
				gameSession->currentLevel->keySet.insert(GLFW_KEY_D);
				if (gameSession->currentLevel->mario->atGround) {
					gameSession->currentLevel->mario->pEntity->velocity->x += velocityChangeValue;
				} else if (gameSession->currentLevel->mario->jumping | gameSession->currentLevel->mario->climbing) {
					gameSession->currentLevel->mario->pEntity->velocity->x += 0.5f * velocityChangeValue;
				}
			} else if (action == GLFW_RELEASE && gameSession->currentLevel->keySet.find(GLFW_KEY_D) != gameSession->currentLevel->keySet.end()) {
				gameSession->currentLevel->keySet.erase(gameSession->currentLevel->keySet.find(GLFW_KEY_D));
				if (gameSession->currentLevel->mario->atGround) {
					gameSession->currentLevel->mario->pEntity->velocity->x -= velocityChangeValue;
				} else if (gameSession->currentLevel->mario->jumping | gameSession->currentLevel->mario->climbing) {
					gameSession->currentLevel->mario->pEntity->velocity->x -= 0.5f * velocityChangeValue;
				}
			}
		} else if (key == GLFW_KEY_A) {
			if (action == GLFW_PRESS) {
				gameSession->currentLevel->keySet.insert(GLFW_KEY_A);
				if (gameSession->currentLevel->mario->atGround) {
					gameSession->currentLevel->mario->pEntity->velocity->x -= velocityChangeValue;
				} else if (gameSession->currentLevel->mario->jumping | gameSession->currentLevel->mario->climbing) {
					gameSession->currentLevel->mario->pEntity->velocity->x -= 0.5f * velocityChangeValue;
				}
			} else if (action == GLFW_RELEASE && gameSession->currentLevel->keySet.find(GLFW_KEY_A) != gameSession->currentLevel->keySet.end()) {
				gameSession->currentLevel->keySet.erase(gameSession->currentLevel->keySet.find(GLFW_KEY_A));
				if (gameSession->currentLevel->mario->atGround) {
					gameSession->currentLevel->mario->pEntity->velocity->x += velocityChangeValue;
				} else if (gameSession->currentLevel->mario->jumping | gameSession->currentLevel->mario->climbing) {
					gameSession->currentLevel->mario->pEntity->velocity->x += 0.5f * velocityChangeValue;
				}
			}
		} else if (key == GLFW_KEY_W && gameSession->currentLevel->canClimb(gameSession->currentLevel->mario)) {
			if (action == GLFW_PRESS) {
				gameSession->currentLevel->keySet.insert(GLFW_KEY_W);
				if (!gameSession->currentLevel->mario->climbing) {
					gameSession->currentLevel->mario->setState(Mario::CLIMBING);
					gameSession->currentLevel->mario->pEntity->velocity->x = 0.0f;
					gameSession->currentLevel->mario->pEntity->velocity->y = 0.5f * velocityChangeValue;
				} else {
					gameSession->currentLevel->mario->pEntity->velocity->y += 0.5f * velocityChangeValue;
				}
			} else if (action == GLFW_RELEASE && gameSession->currentLevel->mario->climbing) {
				gameSession->currentLevel->mario->pEntity->velocity->y -= 0.5f * velocityChangeValue;
			}
		} else if (key == GLFW_KEY_S && gameSession->currentLevel->canClimb(gameSession->currentLevel->mario)) {
			if (action == GLFW_PRESS) {
				gameSession->currentLevel->keySet.insert(GLFW_KEY_S);
				if (!gameSession->currentLevel->mario->climbing) {
					gameSession->currentLevel->mario->setState(Mario::CLIMBING);
					gameSession->currentLevel->mario->pEntity->velocity->x = 0.0f;
					gameSession->currentLevel->mario->pEntity->velocity->y = -0.5f * velocityChangeValue;
				} else {
					gameSession->currentLevel->mario->pEntity->velocity->y -= 0.5f * velocityChangeValue;
				}
			} else if (action == GLFW_RELEASE && gameSession->currentLevel->mario->climbing) {
				gameSession->currentLevel->mario->pEntity->velocity->y += 0.5f * velocityChangeValue;
			}
		} else if (key == GLFW_KEY_SPACE &&!gameSession->currentLevel->mario->jumping && action == GLFW_PRESS) {
			if (gameSession->currentLevel->mario->atGround) {
				gameSession->currentLevel->mario->pEntity->velocity->y += 2.0f * velocityChangeValue;
			}
			gameSession->currentLevel->mario->setState(Mario::JUMPING);
		}
	}

}


double PlayGameState::getElapsedTime() {
	return glfwGetTime();
}

void PlayGameState::decreaseLife() {
	if (gameSession->lifeCount > 0) {
		gameSession->lifeCount--;
	} else {
		playState = OVER;
	}
}

void PlayGameState::addPoints(int points) {
	gameSession->points+=points;
}

void PlayGameState::finishLevel() {

}
