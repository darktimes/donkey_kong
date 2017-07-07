#include <stdexcept>
#include "renderer.hpp"
#include "utils.hpp"
using namespace Renderer;
using namespace Utils;

const float Window::WINDOW_WIDTH = 560.0f;
const float Window::WINDOW_HEIGHT = 800.0f;

Window::Window(int width, int height, std::string wnd_title) {
	this->width = width;
	this->height = height;
	this->wnd_title = wnd_title;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	windowHandle = glfwCreateWindow(width, height, wnd_title.c_str(), nullptr, nullptr);
	
	if (windowHandle == nullptr) {
		throw std::runtime_error("Failed to create GLFW Window");
	}
	glfwMakeContextCurrent(windowHandle);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to init GLEW");
	}

	glViewport(0, 0, width, height); 
	Logger::i("Init-ed window for renderer.");
}

Window::~Window() {
	glfwDestroyWindow(windowHandle);
}

void SpriteRenderer::init() {
	float quadratVertices[] = {
	// positions         //texture coords
		1.0f,  1.0f,    1.0f, 1.0f, // top right
		1.0f,  0.0f,   1.0f, 0.0f, // bottom right
		0.0f,  0.0f,   0.0f, 0.0f, // bottom left
		0.0f,  1.0f,   0.0f, 1.0f  // top left 
	};
	
	float halfedQuadratVertices[] = {
		// positions   //texture coords
		1.0f,  0.5f,   1.0f, 0.5f, // top right
		1.0f,  0.0f,   1.0f, 0.0f, // bottom right
		0.0f,  0.0f,   0.0f, 0.0f, // bottom left
		0.0f,  0.5f,   0.0f, 0.5f  // top left 
	};
	
	addSprite(quadratVertices, std::string("quadratSprite"));
	addSprite(halfedQuadratVertices, std::string("halfedQuadratSprite"));
}

std::map<std::string, int> SpriteRenderer::sprites;

void SpriteRenderer::addSprite(float* vertices, std::string name) {
	
	GLuint VAO, VBO, EBO;
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
    	};
    	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	sprites[name] = VAO;
}
