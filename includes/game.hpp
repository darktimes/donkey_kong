#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <string.h>
#include <memory>
#include <vector>

#include "records.hpp"
#include "renderer.hpp"
#include "gameStates.hpp"

namespace Engine {

	
	class Game {
		public:
			static void init(/*renderer::ERendererType ERendererType*/);
			static void terminate();
			static void run();
			static void processInput(GLFWwindow*, int key, int scancode, int action, int mode);
			static Renderer::Window* getGameWindow();
						
			static void changeState(GameState* state);

		private:
			GameState* currentGameState;
			Renderer::Window* gameWindow;
			static Game* instance;
			
			Game();
			virtual ~Game();

	};
}


