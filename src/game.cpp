#include "game.hpp"
#include "utils.hpp"
#include "shaderProgram.hpp"
#include "resourceManager.hpp"


using namespace Engine;
using namespace Renderer;
using namespace Utils;

Game* Game::instance = nullptr;

void Game::init(/*ERendererType rendererType*/) {
	if (!Game::instance) {
		glfwInit();
		Game::instance = new Game();		
		Logger::i("Game instance initialized.");
		instance->gameWindow = new Window(Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT, "Donkey Kong");
		
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 		
		
		ResourceManager::init();
		SpriteRenderer::init();
		instance->changeState(new MenuGameState());
	} else {
		throw std::runtime_error("Can not re-init a running game.");
	}
}

void Game::terminate() {
	if (Game::instance) {
		glfwTerminate();
		delete Game::instance;
		Logger::i("Game instance terminated.");
	} else {
		throw std::runtime_error("Can not terminate a non-inited game.");
	}
}

 void Game::run() {
	if (Game::instance) {
		Logger::i("Game session launched.");

//		ShaderProgram* shaderProgram = new ShaderProgram(VERTEX_SHADER, FRAGMENT_SHADER);
		while (!instance->currentGameState->exits()) {
			glfwPollEvents();
//			shaderProgram->use();
			instance->currentGameState->tick();
			glfwSwapBuffers(instance->gameWindow->windowHandle);
		}
	} else {
		throw std::runtime_error("Can not run a non-inited game.");
	}
}

void Game::changeState(GameState* state) {
	Logger::i("GameState changed to " + state->getTag() + ".");
	currentGameState = state;
	glfwSetKeyCallback(instance->getGameWindow()->windowHandle, &Game::processInput);
}


Game::Game() {
	
}

Game::~Game() {
	delete gameWindow;
	delete currentGameState;
}


void Game::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) {
	Game::instance->currentGameState->processInput(window, key, scancode, action, mode);
}

Window* Game::getGameWindow() {
	return instance->gameWindow;
}


int main(int argc, char** args) {
	Logger::init(Logger::verbose);
	Game::init();
	Game::run();
	Game::terminate();
	Logger::terminate();
	return 0;
}
