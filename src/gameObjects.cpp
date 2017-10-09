#include "gameObjects.hpp"
#include "views.hpp"
#include "gameStates.hpp"

using namespace Rendering;
using namespace Math;
using namespace Physics;

const float GameObject::blockEdgeLength = Window::WINDOW_HEIGHT / 40.0f;

GameObject::GameObject(vec2f position, vec2f dimensions):
  position(position), dimensions(dimensions) {

}

/**
* Generates the balk block, that actors can walk on.
*/
TerrainBlock* TerrainBlock::genBalkBlock(vec2f position) {
  TerrainBlock* terrainBlock = new TerrainBlock(position, vec2f(blockEdgeLength,
     blockEdgeLength / 2.0f));
  terrainBlock->objView = std::unique_ptr<View>(new SpriteView(terrainBlock,
     std::string("terrainBalk")));
  return terrainBlock;
}
/**
* generates the ladder block, that actors can climb onto
*/
TerrainBlock* TerrainBlock::genLadderBlock(vec2f position, vec2f dimensions) {
  TerrainBlock* terrainBlock = new TerrainBlock(position, dimensions);
  terrainBlock->objView = std::unique_ptr<View>(new SpriteView(terrainBlock,
     std::string("terrainLadder")));
  return terrainBlock;
}

TerrainBlock::TerrainBlock(Math::vec2f position, Math::vec2f dimensions):
 GameObject(position,dimensions) {
}

Actor::Actor(Math::vec2f position, Math::vec2f dimensions):
 GameObject(position,dimensions), faceDirection(FACE_RIGHT), climbing(false) {

}

/**
* Defines whether the actor is staying at the ground
*/
bool Actor::atGround() {
  return !jumping() && physEntity->velocity.y == 0.0f && !climbing;
}

/**
* Defines whether the actor is in the air, jumping
*/
bool Actor::jumping() {
  return physEntity->acceleration.y != 0.0f && !climbing;
}

// bool Actor::climbing() {
//   return !jumping() && climbing;
// }

/**
* Determines whether an actor collides another actor
*/
bool Actor::collides(const Actor* another) {
  return position.x <= another->position.x + another->dimensions.x
   && position.x + dimensions.x >= another->position.x
   && position.y <= another->position.y + another->dimensions.y
   && position.y + dimensions.y >= another->position.y;
}

/**
* Determines if current actor is over the given one, e.g. mario over barrel
*/
bool Actor::over(const Actor* another) {
  return (position.y > another->position.y + another->dimensions.y)
   && jumping()
   && position.y - another->position.y - another->dimensions.y < TerrainBlock::blockEdgeLength * 2.0f
   && position.y - another->position.y - another->dimensions.y > 0
   && position.x <= another->position.x + another->dimensions.x
   && position.x + dimensions.x >= another->position.x;
}

Mario::Mario(Math::vec2f position): Actor(position,
  vec2f(blockEdgeLength * 5.0f / 6.0f, blockEdgeLength * 6.0f / 7.0f)) {
  physEntity = std::unique_ptr<PhysEntity>(new PhysEntity{60.0f});

  objView = std::unique_ptr<View>(new MarioView(this));
  temporalImmunityTimer = 0;
}


Barrel::Barrel(Math::vec2f position): Actor(position,
   vec2f(blockEdgeLength, blockEdgeLength)) {
  physEntity = std::unique_ptr<PhysEntity>(new PhysEntity{50.0f});
  physEntity->velocity.x = ActivePlayGameSubState::barrelMovmentDelta;

  objView = std::unique_ptr<View>(new BarrelView(this));
}
