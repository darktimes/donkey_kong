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
#include <cstdlib>

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
	prevSecondStamp = timer2 = glfwGetTime();
	PhysEngine::init(gameSession->currentLevel);
	srand((int)(glfwGetTime() * 1356));
	timer2_trh  = rand() % 20 / 10.0f;
	god = false;
}

PlayGameState::~PlayGameState() {
	delete gameSession;
}

void PlayGameState::stateTick() {
	if (playState == OVER) {
		Renderer::SpriteRenderer::renderText("GAME OVER", 190.0f, 650.0f, 0.7, Math::vec3<GLfloat>(1.0f, 0.0f, 0.0f));
		Renderer::SpriteRenderer::renderText("SCORE:" + std::to_string(gameSession->points + gameSession->currentBonus), 190.0f, 550.0f, 0.7, Math::vec3<GLfloat>(1.0f, 0.0f, 0.0f));
		Renderer::SpriteRenderer::renderText("To exit press ESC", 150.0f, 450.0f, 0.7, Math::vec3<GLfloat>(1.0f, 0.0f, 0.0f));

	} else if (playState == FINISHED) {
		Renderer::SpriteRenderer::renderText("NEW RECORD", 190.0f, 650.0f, 0.7, Math::vec3<GLfloat>(1.0f, 0.0f, 0.0f));
		Renderer::SpriteRenderer::renderText("SCORE:" + std::to_string(gameSession->points + gameSession->currentBonus), 190.0f, 550.0f, 0.7, Math::vec3<GLfloat>(1.0f, 0.0f, 0.0f));
		Renderer::SpriteRenderer::renderText("YOUR NAME:", 190.0f, 450.0f, 0.7, Math::vec3<GLfloat>(1.0f, 0.0f, 0.0f));
		Renderer::SpriteRenderer::renderText(newPlayerName, 190.0f, 350.0f, 0.7, Math::vec3<GLfloat>(1.0f, 0.0f, 0.0f));

	} else {
		double currentTime = glfwGetTime();

		PhysEngine::tick();
		gameSession->currentLevel->mario->handleAnimationState();

		for (Barrel* barrel: gameSession->currentLevel->barrels) {
			barrel->handleAnimationState();
		}

		for (TerrainBlock* terrainBlock: gameSession->currentLevel->terrainBlocks) {
			terrainBlock->draw();

		}


		if (god && (((int)(currentTime * 1000.0f) / 250) % 2) == 0) {
			gameSession->currentLevel->mario->draw();
		} else if (!god) {
			gameSession->currentLevel->mario->draw();
		}

		if ((currentTime - gameSession->currentLevel->mario->lostLifeTimer) >= 3.0f) {
			god = false;
		}


		for (Barrel* barrel: gameSession->currentLevel->barrels) {
			barrel->draw();
		}

		if (currentTime - prevSecondStamp > 1.0f) {
			prevSecondStamp += 1.0f;
			if (gameSession->currentBonus != 0.0f) {
				gameSession->currentBonus -= 10.0f;
			}
		}

		if (currentTime - timer2 >= timer2_trh) {
			timer2_trh  = rand() % 20 / 10.0f;
			timer2 = currentTime;
			gameSession->currentLevel->barrels.push_back(new Barrel(new Math::vec2<GLfloat>(20.0f, 460.0f)));
		}


		Record* firstRecord = ResourceManager::recordTable->getRecords()[0];
		Renderer::SpriteRenderer::renderText("Score", 0.0f, 780.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.0f,0.0f));
		Renderer::SpriteRenderer::renderText("Max.Record", 200.0f, 780.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.0f, 0.0f));
		Renderer::SpriteRenderer::renderText("Bonus", 450.0f, 780.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.0f, 0.0f));
		Renderer::SpriteRenderer::renderText(std::string((firstRecord->playerName) + ": " + std::to_string(firstRecord->score)), 200.0f, 760.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.5f, 0.0f));
		Renderer::SpriteRenderer::renderText(std::to_string(gameSession->points), 0.0f, 760.0f, 0.5f, Math::vec3<GLfloat>(0.0f, 0.0f, 0.5f));
		Renderer::SpriteRenderer::renderText(std::to_string(gameSession->currentBonus), 450.0f, 760.0f, 0.5f, Math::vec3<GLfloat>(0.0f, 0.0f, 0.5f));
		Renderer::SpriteRenderer::renderText("Lifes", 0.0f, 740.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.0f,0.0f));
		Renderer::SpriteRenderer::renderText(std::to_string(gameSession->lifeCount), 0.0f, 720.0f, 0.5f, Math::vec3<GLfloat>(0.0f, 0.0f, 0.5f));
		Renderer::SpriteRenderer::renderText("Finish", 235.0f, 525.0f, 0.5f, Math::vec3<GLfloat>(0.0f, 1.0f, 0.0f));
	}
}

bool PlayGameState::exits() {
	return GameState::exits();
}

std::string PlayGameState::getTag() {
	return TAG;
}

void PlayGameState::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		Game::changeState(new MenuGameState());
	}
	if (playState == FINISHED) {
		if (action == GLFW_PRESS) {
			if (newPlayerName.size() <= 8 && key != GLFW_KEY_ENTER) {
				if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
					newPlayerName+= (char) (key - GLFW_KEY_A + 65);
				}
			} else {
				ResourceManager::recordTable->addRecord(new Record(gameSession->points + gameSession->currentBonus, newPlayerName));
				ResourceManager::recordTable->saveTable();
				Game::changeState(new MenuGameState());
			}
		}
	} else {
		// GLfloat velocityChangeValue = Physics::movementDelta;
		 if (key == GLFW_KEY_D) {
			if (action == GLFW_PRESS) {
				gameSession->currentLevel->keySet.insert(GLFW_KEY_D);
			} else if (action == GLFW_RELEASE && gameSession->currentLevel->keySet.find(GLFW_KEY_D) != gameSession->currentLevel->keySet.end()) {
				gameSession->currentLevel->keySet.erase(gameSession->currentLevel->keySet.find(GLFW_KEY_D));

			}
		} else if (key == GLFW_KEY_A) {
			if (action == GLFW_PRESS) {
				gameSession->currentLevel->keySet.insert(GLFW_KEY_A);

			} else if (action == GLFW_RELEASE && gameSession->currentLevel->keySet.find(GLFW_KEY_A) != gameSession->currentLevel->keySet.end()) {
				gameSession->currentLevel->keySet.erase(gameSession->currentLevel->keySet.find(GLFW_KEY_A));

			}
		} else if (key == GLFW_KEY_W && gameSession->currentLevel->canClimb(gameSession->currentLevel->mario)) {
			if (action == GLFW_PRESS) {
				gameSession->currentLevel->keySet.insert(GLFW_KEY_W);
			} else if (action == GLFW_RELEASE && gameSession->currentLevel->keySet.find(GLFW_KEY_W) != gameSession->currentLevel->keySet.end() /* && gameSession->currentLevel->mario->climbing*/) {
				gameSession->currentLevel->keySet.erase(gameSession->currentLevel->keySet.find(GLFW_KEY_W));
			}
		} else if (key == GLFW_KEY_S && gameSession->currentLevel->canClimb(gameSession->currentLevel->mario)) {
			if (action == GLFW_PRESS) {
				gameSession->currentLevel->keySet.insert(GLFW_KEY_S);
			} else if (action == GLFW_RELEASE && gameSession->currentLevel->keySet.find(GLFW_KEY_S) != gameSession->currentLevel->keySet.end()/*&& gameSession->currentLevel->mario->climbing*/) {
				gameSession->currentLevel->keySet.erase(gameSession->currentLevel->keySet.find(GLFW_KEY_S));
				// gameSession->currentLevel->mario->pEntity->velocity->y += 0.5f * velocityChangeValue;
			}
		} else if (key == GLFW_KEY_SPACE /*&&!gameSession->currentLevel->mario->jumping && action == GLFW_PRESS*/) {
			if (action == GLFW_PRESS) {
				gameSession->currentLevel->keySet.insert(GLFW_KEY_SPACE);
			} else if (action == GLFW_RELEASE && gameSession->currentLevel->keySet.find(GLFW_KEY_SPACE) != gameSession->currentLevel->keySet.end()) {
				gameSession->currentLevel->keySet.erase(gameSession->currentLevel->keySet.find(GLFW_KEY_SPACE));
			}
			// if (gameSession->currentLevel->mario->atGround) {
			// 	gameSession->currentLevel->mario->pEntity->velocity->y += 2.0f * velocityChangeValue;
			// }
			// gameSession->currentLevel->mario->setState(Mario::JUMPING);
		} else if (key == GLFW_KEY_X && action == GLFW_PRESS) {
			gameSession->currentLevel->barrels.push_back(new Barrel(new Math::vec2<GLfloat>(20.0f, 460.0f)));
		}
	}

}


double PlayGameState::getElapsedTime() {
	return glfwGetTime();
}

void PlayGameState::decreaseLife() {
	if (gameSession->lifeCount > 0 ) {
		if (!god) {
			god = true;
			gameSession->currentLevel->mario->lostLifeTimer = glfwGetTime();
			gameSession->lifeCount--;
		}
	} else {
		playState = OVER;
	}
}

void PlayGameState::addPoints(int points) {
	gameSession->points+=points;
}

void PlayGameState::finishLevel() {
	if ((ResourceManager::recordTable->isNewScore(gameSession->points + gameSession->currentBonus))) {
			playState = FINISHED;
	} else {
		playState = OVER;
	}

}
