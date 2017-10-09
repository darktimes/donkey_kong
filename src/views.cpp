#include "views.hpp"
#include "utils.hpp"
#include "gameObjects.hpp"
using namespace Rendering;
using namespace Utils;

View::View(GameObject* parent) {
  this->parent = parent;
  Renderer::getInstance()->addView(this);
}

View::~View() {
  Renderer::getInstance()->removeView(this);

}

SpriteView::SpriteView(GameObject* parent, std::string textureName):
 View(parent) {
  this->textureName = textureName;
}

std::string SpriteView::getCurrentTextureName() {
  return textureName;
}

Math::mat4 SpriteView::getModelMatrix() {
  Math::mat4 model;
  model.scaleX(parent->dimensions.x);
  model.scaleY(parent->dimensions.y);
	model.translate(parent->position);
  return model;
}

void SpriteView::draw() {
  Renderer::getInstance()->drawSprite(this);
}

TextView::TextView(std::string text, Math::vec2f position, float scale,
 Math::vec3f color):View(nullptr), text(text), position(position), scale(scale), color(color) {

}

void TextView::draw() {
  Renderer::getInstance()->drawText(this);
}

TextView::~TextView() {

}

AnimatedSpriteView::AnimatedSpriteView(GameObject* parent): SpriteView(parent, "") {

}

void AnimatedSpriteView::addAnimation(Animation* animation, std::string name) {
  animations[name] = std::unique_ptr<Animation>(animation);
}

void AnimatedSpriteView::draw() {
  update();
  SpriteView::draw();
}

std::string AnimatedSpriteView::getCurrentTextureName() {
  return animations[currentAnimationName]->getCurrentTexture();
}

MarioView::MarioView(Mario* parent): AnimatedSpriteView(parent), flickerTimer(0) {
  addAnimation(new Animation(std::vector<std::string> {"runRight1", "runRight2"}), "animRunRight");
	addAnimation(new Animation(std::vector<std::string> {"runLeft1", "runLeft2"}), "animRunLeft");
	addAnimation(new Animation(std::vector<std::string> {"climb1", "climb2"}), "animClimb");
	addAnimation(new Animation(std::vector<std::string> {"jumpLeft"}), "animJumpLeft");
	addAnimation(new Animation(std::vector<std::string> {"jumpRight"}), "animJumpRight");
}

void MarioView::update() {
  Mario* mario = static_cast<Mario*>(parent);
  if (mario->atGround()) {
    if (mario->faceDirection == Actor::FACE_RIGHT) {
      currentAnimationName = "animRunRight";
    } else {
      currentAnimationName = "animRunLeft";
    }
    if (mario->physEntity->velocity.x == 0) {
      animations[currentAnimationName]->stopped = true;
    } else {
      animations[currentAnimationName]->stopped = false;
    }
  } else if (mario->jumping()) {
    if (mario->faceDirection == Actor::FACE_RIGHT) {
      currentAnimationName = "animJumpRight";
    } else {
      currentAnimationName = "animJumpLeft";
    }
  } else if (mario->climbing) {
    currentAnimationName = "animClimb";
    if (mario->physEntity->velocity.y == 0) {
      animations[currentAnimationName]->stopped = true;
    } else {
      animations[currentAnimationName]->stopped = false;
    }
  }
}

void MarioView::draw() {
  update();
  Mario* mario = static_cast<Mario*>(parent);
  if (mario->temporalImmunity && (((int)(Utils::getTime() * 1000.0f) / 250) % 2) == 0) {
    SpriteView::draw();
  } else if (!mario->temporalImmunity){
    SpriteView::draw();
  }
}

BarrelView::BarrelView(Barrel* parent): AnimatedSpriteView(parent) {
  addAnimation(new Animation(std::vector<std::string> {"barrel1", "barrel2", "barrel3", "barrel4"}), "animBarrelRoll");
  addAnimation(new Animation(std::vector<std::string> {"barrelFront1", "barrelFront2"}), "animBarrelRollFront");
}

void BarrelView::update() {
  if (static_cast<Barrel*>(parent)->climbing) {
    currentAnimationName = "animBarrelRollFront";
  } else {
    currentAnimationName = "animBarrelRoll";
  }
}
