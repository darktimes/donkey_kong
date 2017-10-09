#pragma once

#include <memory>

#include "math.hpp"
#include "physics.hpp"
#include "renderer.hpp"

namespace Rendering {
  class View;
};

/**
* GameObject class represents game objects, that are contained within
* the game. GameObject can have an objView attached, that makes the
* object possible to render.
*/
class GameObject {
  public:
     static const float blockEdgeLength;
    ~GameObject() {};
    Math::vec2f position;
    Math::vec2f dimensions;
    std::unique_ptr<Rendering::View> objView;
  protected:
    GameObject(Math::vec2f position, Math::vec2f dimensions);
};

/**
* Doesn't differ much from parent class, besides the static functions, which
* are not desired in actor class. So, moving them in here for sake of
* classes clarrification.
*/
class TerrainBlock: public GameObject {
  public:
    static TerrainBlock* genBalkBlock(Math::vec2f position);
    static TerrainBlock* genLadderBlock(Math::vec2f position,
     Math::vec2f dimensions);
  private:
    TerrainBlock(Math::vec2f position, Math::vec2f dimensions);
};

/**
* GameObject, that is expected to be simulated with physical values.
*/
class Actor: public GameObject {
  public:
    enum FaceDirection {FACE_LEFT, FACE_RIGHT};

    /*actor's state reflecting methods */
    bool atGround();
    bool jumping();

    bool collides(const Actor*);
    bool over(const Actor*);

    std::unique_ptr<Physics::PhysEntity> physEntity;
    FaceDirection faceDirection;
    bool climbing;
  protected:
    Actor(Math::vec2f position, Math::vec2f dimensions);

};

class Mario: public Actor {
  public:
    Mario(Math::vec2f position);
    bool temporalImmunity;
    double temporalImmunityTimer;
};

class Barrel: public Actor {

  public:
    Barrel(Math::vec2f position);
    double scoreTimer;
};
