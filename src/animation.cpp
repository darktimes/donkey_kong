#include <GLFW/glfw3.h>
#include "animation.hpp"

using namespace Engine;

Animation::Animation(std::vector<std::string> textures, double fraction) {
	this-> textures = textures;
	this->fraction = fraction;
	lastTick = 0;
	currentIndex = 0;
	stopped = true;
}

std::string Animation::getCurrentTexture() {
    double currentTime = glfwGetTime();
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


