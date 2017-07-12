#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>

#include "math.hpp"

namespace Renderer {
	class Window {
		public:
			static const float WINDOW_WIDTH;
			static const float WINDOW_HEIGHT;

			GLFWwindow* windowHandle;
			int width;
			int height;
			Window(int width,int height, std::string wnd_title);
			virtual ~Window();
		protected:

			std::string wnd_title;
	};

	class SpriteRenderer {
		public:
    	static unsigned int textVBO, textVAO, textEBO;
			static std::map<std::string, int> sprites;
			static void init();
			static void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, Math::vec3<GLfloat> color);
		private:
			static void addSprite(float* verticies, std::string name);
	};
}
