#include <algorithm>
#include "renderer.hpp"
#include "openGlRenderer.hpp"
#include "vulkanRenderer.hpp"
#include "utils.hpp"

using namespace Math;
using namespace Rendering;
using namespace Utils;

std::unique_ptr<Renderer> Renderer::instance = nullptr;
RendererType Renderer::prefRendererType = NOT_SET;

RawTexture::RawTexture(vec2ui size, std::vector<unsigned char> *v): size (size),
data(*v) {
  // this->data = new unsigned char[size.x * size.y * 3];
  // for (unsigned i = 0; i<= size.x * size.y * 3; i++) {
  //   this->data[i] = data[i];
  // }
}

RawTexture::~RawTexture() {

}

Window::Window(vec2ui size, std::string title):
 size(size), title(title) {

 	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

 	windowHandle = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

 	if (windowHandle == nullptr) {
 		throw std::runtime_error("Failed to create GLFW Window");
 	}

}

void Renderer::selectRendererType(RendererType rendererType) {
  Renderer::prefRendererType = rendererType;
  Logger::s("Prefered renderer set to " + (prefRendererType == OPENGL ?
   std::string("OpenGL") : prefRendererType == VULKAN ? std::string("Vulkan") :
   std::string( "Unknown")));
}

const std::unique_ptr<Renderer>& Renderer::getInstance() {
  if (!Renderer::instance) {
    if (prefRendererType == VULKAN) {
		throw std::runtime_error("Vulkan renderer is not supported.");
      instance = std::unique_ptr<Renderer>(new VulkanRenderer());
    } else {
      instance = std::unique_ptr<Renderer>(new OpenGlRenderer());
    }
    // else {
    //   throw std::runtime_error("Unknown rendererType");
    // }
  }
  return instance;
}

void Renderer::loadTextures() {
  loadTexture(std::string("terrain_balk_16"), std::string("terrainBalk"));
  loadTexture(std::string("terrain_ladder_16"), std::string("terrainLadder"));
  loadTexture(std::string("barrel1_front_16"), std::string("barrelFront1"));
  loadTexture(std::string("barrel2_front_16"), std::string("barrelFront2"));
  loadTexture(std::string("barrel1_16"), std::string("barrel1"));
  loadTexture(std::string("barrel2_16"), std::string("barrel2"));
  loadTexture(std::string("barrel3_16"), std::string("barrel3"));
  loadTexture(std::string("barrel4_16"), std::string("barrel4"));
  loadTexture(std::string("runRight1_16"), std::string("runRight1"));
  loadTexture(std::string("runRight2_16"), std::string("runRight2"));
  loadTexture(std::string("runLeft1_16"), std::string("runLeft1"));
  loadTexture(std::string("runLeft2_16"), std::string("runLeft2"));
  loadTexture(std::string("jumpLeft_16"), std::string("jumpLeft"));
  loadTexture(std::string("jumpRight_16"), std::string("jumpRight"));
  loadTexture(std::string("climb1_16"), std::string("climb1"));
  loadTexture(std::string("climb2_16"), std::string("climb2"));
}

void Renderer::loadFont(const std::string& fontName) {
  std::unique_ptr<std::string> fileString = std::unique_ptr<std::string>(
    readFile(*Utils::PATH_FONTS + fontName));
  unsigned start = 0;
  unsigned end = 0;
  std::string colon = ";";
  if (!fileString->empty()) {
    for (unsigned k = 0; k < 256; k++) {
      GLfloat width = nextToken(start, end, *fileString);
      GLfloat height = nextToken(start, end, *fileString);
      GLfloat left = nextToken(start, end, *fileString);
      GLfloat top = nextToken(start, end, *fileString);
      GLuint advance = nextToken(start, end, *fileString);

      unsigned dim = (unsigned)((width == height) && (width == 0) ? 0 : ((width == 0 ? 1 : width) * (height == 0 ? 1 : height)));
      std::vector<unsigned char> *data = new std::vector<unsigned char>();
      for (unsigned i = 0; i < dim; i++) {
        end = fileString->find(colon, start);
        unsigned char c = nextToken(start, end, *fileString) * 255;
        data->push_back(c);
      }
      Math::vec2f size = Math::vec2f(width, height);
      Math::vec2f bearing = Math::vec2f(left, top);
      generateFontChar(k, size, bearing, advance, data);
    }
  //static_cast<OpenGlRenderer*>(this)->test("open-sans");
   }
}

void Renderer::loadTexture(const std::string& fileName,
   const std::string& textureName) {
  generateTexture(textureName, parseTexture(fileName));
}

void Renderer::addView(View* view) {
  drawList.push_back(view);
}

void Renderer::removeView(View* view) {
  auto it = std::find(drawList.begin(), drawList.end(), view);
  if (it != drawList.end())
    drawList.erase(it);

}

const std::unique_ptr<Window>& Renderer::getWindow() {
  return window;
}

int Renderer::nextToken(unsigned& start, unsigned& end, std::string& str) {
	end = str.find(";", start);
	int value = std::stoi(str.substr(start, end - start));
	start = end + 1;
	return value;
}
