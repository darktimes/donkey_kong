#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "math.hpp"
#include "gameMisc.hpp"

namespace Engine {

	struct GameSession;

	class GameState {
		public:
			GameState();
			void tick();

			virtual void stateTick() = 0;
			virtual bool exits() = 0;
			virtual std::string getTag() = 0;

			virtual void processInput(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;
			virtual ~GameState() {};
	};


	class MenuGameState : public GameState {
		static const std::string TAG;
	
		public:
		
		    	unsigned int VBO, VAO, EBO;
			MenuGameState();
			void stateTick() override;
			bool exits() override;
			std::string getTag() override;
			void processInput(GLFWwindow* window, int key, int scancode, int action, int mode) override;
			void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, Math::vec3<GLfloat> color);
	};
	
	class IPlayGameStateCallbacks {
		public:
			virtual double getElapsedTime() = 0;
			virtual void decreaseLife() = 0;
			virtual void addPoints(int points) = 0;
			virtual void finishLevel() = 0;
			~IPlayGameStateCallbacks() {};
	};
	
	class PlayGameState : public GameState, public IPlayGameStateCallbacks {
		static const std::string TAG; 
	
		public:
			PlayGameState();
			void stateTick() override;
			bool exits() override;
			std::string getTag();
			void processInput(GLFWwindow* window, int key, int scancode, int action, int mode) override;
			
			double getElapsedTime() override;
			void decreaseLife() override;
			void addPoints(int points) override;
			void finishLevel() override;
			
		private:
			GameSession* gameSession;
	};
}
