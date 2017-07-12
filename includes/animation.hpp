#pragma once
#include <vector>
#include <string>

namespace Engine {
    class Animation {
        public:
		Animation(std::vector<std::string> textures, double fraction = 0.250f);
		std::string getCurrentTexture();
		bool stopped;

        private:
		std::vector<std::string> textures;
		double lastTick;
		double fraction;
		unsigned currentIndex;
    };
}
