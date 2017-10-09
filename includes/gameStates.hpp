#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

#include "views.hpp"
#include "gameSession.hpp"
#include "renderer.hpp"
#include "gameLevel.hpp"
#include "recordTable.hpp"


struct GameSession;

class Level;

namespace Rendering {
  class View;
  class TextView;
};

/**
* GameState class is a merge of controller and model components. It
* manipulates the game state and runs the game behavior.
*/
class GameState {
  public:
    GameState() {};
    virtual ~GameState() {};

    virtual bool exits();

    virtual std::string getTag() = 0;
    virtual void stateTick() = 0;
    virtual void processInput(GLFWwindow* window, int key, int scancode,
       int action, int mode) = 0;
  protected:
    std::vector<std::unique_ptr<Rendering::View>> viewList;
};

/**
* MenuGameState controlls the main menu. Main menu displays the record table and
* either exits the game or starts a new one.
*/
class MenuGameState: public GameState {
  static const std::string TAG;

  public:
    MenuGameState();
    std::string getTag() override;
    void stateTick() override;
    void processInput(GLFWwindow* window, int key, int scancode, int action,
       int mode) override;
};

class PlayGameSubState;

/**
* PlayGameState handls the game itself. It has 3 sub-states:
* 1)Active - game is running. Mario is moving, barrels are falling etc.
* 2)Over - game over title is screened and subsequent esc-press returns to menu
* 3)Finished - game is over, but with a new record that can be entered to the
* record table with provided name.
*/
class PlayGameState : public GameState {
  static const std::string TAG;

  public:
    PlayGameState();
    ~PlayGameState();
    void stateTick() override;


    std::string getTag() override;
    void processInput(GLFWwindow* window, int key, int scancode, int action, int mode) override;
    void changeSubState(PlayGameSubState* substate);
    std::unique_ptr<PlayGameSubState> subState;

  private:

    // bool god;
    // double prevSecondStamp;
    // double timer2;
    // double timer2_trh;
    //
    // double marioLostLifeTime;


};

class PlayGameSubState: public GameState {
  protected:
    PlayGameSubState(PlayGameState* parent);
    PlayGameState* parent;
};

/**
* Main state, where the game is handled.
*/
class ActivePlayGameSubState: public PlayGameSubState {
  public:
    static const float collisionBias;
    static const float fallingConstant;
    static const float movementDelta;
    static const float barrelMovmentDelta;
    static const float climbingDelta;
    static const float barrelClimbingDelta;

    std::unique_ptr<GameSession> gameSession;
    std::unique_ptr<Level> currentLevel;

    ActivePlayGameSubState(PlayGameState*);
    std::string getTag() override;
    void stateTick() override;
    void processInput(GLFWwindow* window, int key, int scancode, int action,
     int mode) override;

     void setLevel(Level*);
     void processCurrentLevel();
     void moveMario();
     void moveBarrel(Barrel*);
     void directActorLeft(Actor*);
     void directActorRight(Actor*);
     void directActorUp(Actor*);
     void directActorDown(Actor*);
     void jumpActor(Actor*);

     void stopActorLeft(Actor*);
     void stopActorRight(Actor*);
     void stopActorUp(Actor*);
     void stopActorDown(Actor*);

     ~ActivePlayGameSubState();
  private:
    int fpsCounter;
    int fpsDisplayCounter;
    double fpsTimer;
    double bonusTimer;
    double moveTimer;



    std::unique_ptr<Rendering::TextView> scoreText;
    std::unique_ptr<Rendering::TextView> currentBonusText;
    std::unique_ptr<Rendering::TextView> lifeText;
    std::unique_ptr<Rendering::TextView> fpsText;



};

/**
* State where the game over title is screened
*/
class OverPlayGameSubState: public PlayGameSubState {
  public:
    OverPlayGameSubState(PlayGameState*, int score);
    std::string getTag() override;
    void stateTick() override;
    void processInput(GLFWwindow* window, int key, int scancode, int action,
     int mode) override;
};

/**
* This state screens the game over title and gives the
* possibility to enter records holder name to the record table
*/
class OverRecordPlayGameSubState: public PlayGameSubState {
  public:
    OverRecordPlayGameSubState(PlayGameState*, int score);
    std::string getTag() override;
    void stateTick() override;
    void processInput(GLFWwindow* window, int key, int scancode, int action,
     int mode) override;
  private:
    std::unique_ptr<Rendering::TextView> recordHolderNameText;
    std::string recordHolderName;
    int score;
};
