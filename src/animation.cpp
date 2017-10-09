#include "utils.hpp"
#include "animation.hpp"

using namespace Rendering;

Animation::Animation(std::vector<std::string> textures, double fraction) {
	this-> textures = textures;
	this->fraction = fraction;
	lastTick = 0;
	currentIndex = 0;
	stopped = false;
}

/**
* Gets current animation texture depending on elapsed time
*/
std::string Animation::getCurrentTexture() {
    double currentTime = Utils::getTime();
    if (lastTick == 0 || stopped) {
        lastTick = currentTime;
        return textures[0];
    } else {
      if (currentTime - lastTick >= fraction) {
          lastTick += fraction;
          currentIndex = (currentIndex + 1) % textures.size();
      }
      return textures[currentIndex];
    }
}
