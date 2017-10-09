#pragma once

#include <vector>
#include <map>
#include "renderer.hpp"
#include "math.hpp"
#include "gameObjects.hpp"
#include "animation.hpp"

class GameObject;
class Mario;
class Barrel;

namespace Rendering {
  /**
  * View class represents an observable instance of something on
  * the screen. It automatically adds itself to the renderlist
  * by creation and removes itself from it by destruction.
  * Designed to be subclassed.
  */
  class View {
    public:
      virtual void draw() = 0;
      virtual ~View();
      GameObject* parent;
    protected:
      View(GameObject*);

  };

  /**
  * Classic sprite, that displays a texture on a quad.
  */
  class SpriteView: public View {
    public:
      void draw() override;
      virtual Math::mat4 getModelMatrix();
      SpriteView(GameObject* parent, std::string textureName);
      virtual std::string getCurrentTextureName();
    private:
      std::string textureName;
  };

  /**
  * Sprite, that has multiple animations. The update function is designed to
  * set the proper animation depending on the view's parent state.
  */
  class AnimatedSpriteView: public SpriteView {
    public:
      AnimatedSpriteView(GameObject* parent);
      void addAnimation(Animation*, std::string);
      void draw() override;
      virtual void update() = 0;
      virtual std::string getCurrentTextureName() override;
    protected:
      std::string currentAnimationName;
      std::map<std::string, std::unique_ptr<Animation>> animations;
  };

  class MarioView: public AnimatedSpriteView {
  public:
    MarioView(Mario* parent);
    void draw() override;
    void update() override;
    double flickerTimer;

  };

  class BarrelView: public AnimatedSpriteView {
  public:
    BarrelView(Barrel* parent);
    void update() override;

  };

  /**
  * Designed to wrap the text rendering stuff
  */
  class TextView: public View {
  public:
    TextView(std::string text, Math::vec2f position, float scale,
     Math::vec3f color);
    std::string text;
    Math::vec2f position;
    float scale;
    Math::vec3f color;

    void draw() override;
    ~TextView();
  };
};
