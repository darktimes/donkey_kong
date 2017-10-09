#pragma once
#include <vector>
#include <string>

namespace Rendering {

  /**
  * This class holds information for a animation.
  */
  class Animation {
    public:
      Animation(std::vector<std::string> textures, double fraction = 0.250f);
      std::string getCurrentTexture();
      /*whether the animation should be stopped playing, and the first image should be shown*/
      bool stopped;

    private:
      std::vector<std::string> textures;
      double lastTick;
      double fraction;
      unsigned currentIndex;
  };

}
