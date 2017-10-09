#include "game.hpp"
#include "renderer.hpp"
#include "utils.hpp"

using namespace Rendering;
using namespace Utils;

/**
* Returns true, if given string is contained in the
* passed char* array
*/
bool argumentPresent(const std::string& searched_term, int argc,
   char** args) {
  for (auto i = 0; i < argc; i++) {
    std::string temp(args[i]);
    if (temp.compare(searched_term) == 0)
      return true;
  }
  return false;
}

int main(int argc, char** args) {
  Game& gameInstance = Game::getInstance();
  if (argumentPresent("-test", argc, args)) {
    gameInstance.init(Game::TEST);
    gameInstance.test();
  } else {
    if (argumentPresent("-vulkan", argc, args))
      Renderer::selectRendererType(VULKAN);
    else
      Renderer::selectRendererType(OPENGL);
    gameInstance.init(Game::DEFAULT);
    gameInstance.run();
  }
  gameInstance.terminate();
  return 0;
}
