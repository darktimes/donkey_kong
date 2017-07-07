#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>

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
			static std::map<std::string, int> sprites;
			static void init();	
		private:
			static void addSprite(float* verticies, std::string name);	
	};
}
