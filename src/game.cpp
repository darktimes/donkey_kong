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


void Game::test() {
  changeState(new PlayGameState());

  ActivePlayGameSubState* state = static_cast<ActivePlayGameSubState*>(
    static_cast<PlayGameState*>(currentGameState.get()
  )->subState.get());

  Level* currentLevel = state->currentLevel.get();

  Mario* mario = currentLevel->mario.get();

  currentGameState->stateTick();
  Utils::fixedTime = 0.2f;
  currentGameState->stateTick();

  /*mario should be at the start in the air, jumping*/
  assert(mario->climbing == false);
  assert(mario->jumping() == true);
  assert(mario->atGround() == false);

  for (int i = 0; i < 100; i++) {
    Utils::fixedTime+= 0.01f;
    currentGameState->stateTick();
  }

  /*after a while though, he must be at ground*/
  assert(mario->atGround() == true);
  assert(mario->climbing == false);
  assert(mario->jumping() == false);

  /*we move mario left and expect him to face the game border*/
  state->directActorLeft(mario);

  for (int i = 0; i < 100; i++) {
    Utils::fixedTime+= 0.01f;
    currentGameState->stateTick();
  }

  /*test, if mario reached the left border*/
  assert(mario->position.x == 0);


  state->directActorRight(mario);

  for (int i = 0; i < 1000; i++) {
    Utils::fixedTime+= 0.01f;
    currentGameState->stateTick();
  }

  /*test if mario is not moving, after reaching the right border*/
  assert(mario->physEntity->velocity.x == 0);

  /*move mario to the first ladder*/
  mario->position.x = GameObject::blockEdgeLength * 23.0f;

  state->directActorUp(mario);

  Utils::fixedTime+= 0.01f;
  currentGameState->stateTick();

  /*test, if mario climbs after issueing the move upwards command on ladder*/
  assert(mario->climbing == true);

  for (int i = 0; i < 1000; i++) {
    Utils::fixedTime+= 0.01f;
    currentGameState->stateTick();
  }

  /*test if mario finished climbing after reaching the top of the balk*/
  assert(mario->atGround() == true);

  Barrel* barrel = new Barrel(mario->position);
  barrel->physEntity->velocity.x = 0;
  barrel->physEntity->velocity.y = 0;

  /*test if mario collides a barrel*/
  assert(mario->collides(barrel) == true);

  mario->position.y += GameObject::blockEdgeLength * 1.5f;
  Utils::fixedTime+= 0.01f;
  currentGameState->stateTick();

  /*check if mario can score above the barrel*/
  assert(mario->over(barrel) == true);

  delete barrel;

  /*check if barrel changes direction after hitting the wall */
  barrel = new Barrel(Math::vec2f(20.0f, 600.0f));
  barrel->physEntity->velocity.x = -ActivePlayGameSubState::movementDelta * 1.5f;
  barrel->physEntity->acceleration.y = ActivePlayGameSubState::fallingConstant;

  for (int i = 0; i < 2000; i++) {
    Utils::fixedTime+= 0.01f;
    currentGameState->stateTick();
  }

  assert(barrel->faceDirection == Actor::FACE_RIGHT);
  delete barrel;
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
