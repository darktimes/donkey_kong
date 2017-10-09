#pragma once

#include <vector>
#include <set>

#include "gameStates.hpp"
#include "gameObjects.hpp"
#include "math.hpp"
#include "renderer.hpp"

class PlayGameState;
class ActivePlayGameSubState;

/**
* The Level class contains scene belonging objects. Level is the part of the model.
* It has a tick method, where specific level events should be processed.
*/
class Level {
  public:
    unsigned startPointBonus;
    Math::vec2ui size;
    Math::vec2f finishAreaPosition;
    Math::vec2f finishAreaDimensions;
    std::unique_ptr<Mario> mario;
    std::string levelName;

    std::vector<std::unique_ptr<TerrainBlock>> terrainBlocks;
    std::vector<std::unique_ptr<TerrainBlock>> ladders;
    std::vector<std::unique_ptr<Barrel>> barrels;
    bool canClimb(Actor*);
    virtual void tick() = 0;
    virtual ~Level() {};

    Level(unsigned startPointBonus, Math::vec2ui size,
       Math::vec2f finishAreaPosition,
       Math::vec2f finishAreaDimensions,
       std::string levelName);
  protected:
    double barrelSpawnTimer;
    double barrelSpawnThreshold;

};

class LevelOne: public Level {
  public:
    LevelOne();
    void tick() override;
    virtual ~LevelOne() {};
};
