#include <string>
#include "game.hpp"
#include "renderer.hpp"
#include "utils.hpp"
#include "gameLevel.hpp"
#include "gameObjects.hpp"
#include <cassert>

using namespace Utils;
using namespace Rendering;
using namespace Math;

const unsigned Window::WINDOW_WIDTH   = 560;
const unsigned Window::WINDOW_HEIGHT  = 800;

Game& Game::getInstance() {
  static Game instance;
  return instance;
}

/**
* Initializes the game modules depending on mode.
* On testing mode, rendering stuff is disabled
*/
void Game::init(GameMode gameMode) {
  recordTable.readTable();
  this->gameMode = gameMode;
  if (gameMode == DEFAULT) {
    glfwInit();
    Renderer::getInstance()->createWindow(
    vec2ui(Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT),
    "DonkeyKong Game");
    Renderer::getInstance()->init();
    Renderer::getInstance()->loadTextures();
    Renderer::getInstance()->loadFont("open-sans");
    //controller init
  }
  changeState(new MenuGameState());
}

/*
* static wrapper for glfw processInput callback
*/
void Game::processInputStaticWrapper(GLFWwindow* window, int key, int scancode,
   int action, int mode) {
  Game::getInstance().currentGameState->processInput(window, key, scancode,
     action, mode);
}

/**
* Changes the current game state, updating keyboard callbacks
*/
void Game::changeState(GameState* gameState) {
  currentGameState.reset(gameState);
  if (gameMode == DEFAULT) {
    glfwSetKeyCallback(Renderer::getInstance()->getWindow()->windowHandle,
     &processInputStaticWrapper);
    Logger::i("Changed gameState to " + currentGameState->getTag());
  }
}

/**
* Main game loop
*/
void Game::run() {
  while (!currentGameState->exits()) {
    if (gameMode == DEFAULT) {
      glfwPollEvents();
      Renderer::getInstance()->draw();
    }
    currentGameState->stateTick();
  }
}

/**
* Terminates the game and cleans resources.
*/
void Game::terminate() {
  recordTable.saveTable();
  if (gameMode == DEFAULT) {
    Renderer::getInstance()->terminate();
    glfwTerminate();
  }
}
