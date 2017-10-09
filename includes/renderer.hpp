#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <memory>

#include "math.hpp"
#include "gameObjects.hpp"
#include "views.hpp"

class GameObject;

namespace Rendering {

  class View;
  class SpriteView;
  class TextView;

  enum RendererType {NOT_SET = -1, OPENGL = 0, VULKAN = 1};

  struct RawTexture {
    Math::vec2ui size;
    std::vector<unsigned char> data;
    RawTexture(Math::vec2ui, std::vector<unsigned char> *v);
    ~RawTexture();
  };

  /**
  * A Window wrapper class to access relevant information with less
  * overhead.
  */
  class Window {
    friend class Renderer;

    public:
      GLFWwindow* windowHandle;
      static const unsigned WINDOW_WIDTH;
      static const unsigned WINDOW_HEIGHT;
      virtual ~Window() {};
      Window(Math::vec2ui, std::string);
      Math::vec2ui size;

    protected:

      std::string title;
  };

  /**
  * An abstract renderer, that exposes the needed rendering functionality.
  */
  class Renderer {
    friend class View;
    public:

      static const std::unique_ptr<Renderer>& getInstance();
      static void selectRendererType(RendererType);


      Renderer(Renderer const&)         = delete;
      void operator=(Renderer const&)   = delete;

      virtual void init() = 0;
      virtual void draw() = 0;
      virtual void terminate() = 0;
      virtual void createWindow(Math::vec2ui, std::string) = 0;
      virtual void generateTexture(const std::string&, RawTexture*) = 0;
      virtual void generateFontChar(unsigned char c, Math::vec2f size,
       Math::vec2f bearing, unsigned advance, std::vector<unsigned char>*) = 0;
      virtual ~Renderer() {};
      virtual void drawSprite(SpriteView*) = 0;
      virtual void drawText(TextView*) = 0;

      const std::unique_ptr<Window>& getWindow();
      void loadTextures();
      void loadFont(const std::string& fontName);

    protected:
      static RendererType prefRendererType;
      static std::unique_ptr<Renderer> instance;

      std::vector<View*> drawList;
      std::unique_ptr<Window> window;

      Renderer() {};
      void addView(View*);
      void removeView(View*);
      int nextToken(unsigned& start, unsigned& end, std::string& str);
    private:
      void loadTexture(const std::string& fileName,
        const std::string& textureName);
  };
};
