
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "recordTable.hpp"
#include "gameStates.hpp"

/**
* Game class wraps all functionality of the game, implemented as singleton.
*/
class Game {
public:
  enum GameMode {TEST = 0, DEFAULT = 1};
  static Game& getInstance();

  Game(Game const&)            = delete;
  void operator=(Game const&)  = delete;

  void init(GameMode);
  void run();
  void test();
  void terminate();
  void changeState(GameState*);

  static void processInputStaticWrapper(GLFWwindow* window, int key,
     int scancode, int action, int mode);

  RecordTable recordTable;
  GameMode gameMode;
private:
  std::unique_ptr<GameState> currentGameState;


  Game() {}
};
