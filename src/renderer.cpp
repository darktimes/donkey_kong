#include <stdexcept>
#include "renderer.hpp"
#include "utils.hpp"
#include "resourceManager.hpp"
#include "game.hpp"

using namespace Renderer;
using namespace Utils;


const float Window::WINDOW_WIDTH = 560.0f;
const float Window::WINDOW_HEIGHT = 800.0f;

std::map<std::string, int> SpriteRenderer::sprites;

unsigned int SpriteRenderer::textVBO, SpriteRenderer::textVAO, SpriteRenderer::textEBO;

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

	float halfedQuadratVertices2[] = {
		// positions   //texture coords
		0.5f,  1.0f,   0.5f, 1.0f, // top right
		0.5f,  0.0f,   0.5f, 0.0f, // bottom right
		0.0f,  0.0f,   0.0f, 0.0f, // bottom left
		0.0f,  1.0f,   0.0f, 1.0f  // top left 
	};
	
	addSprite(quadratVertices, std::string("quadratSprite"));
	addSprite(halfedQuadratVertices, std::string("halfedQuadratSprite"));

	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Engine::ResourceManager::shaders["projectionShader"]->addUniformInt("texture1", 0, true);
	Engine::ResourceManager::shaders["projectionShader"]->addUniformMatrix4("projection", Math::ortho(0.0f, Engine::Game::getGameWindow()->width,
		 0.0f, Engine::Game::getGameWindow()->height, -1.0f, 1.0f), true);
	Engine::ResourceManager::shaders["textShader"]->addUniformMatrix4("projection", Math::ortho(0.0f, Engine::Game::getGameWindow()->width,
		 0.0f, Engine::Game::getGameWindow()->height, -1.0f, 1.0f), true);
}

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

void SpriteRenderer::renderText(std::string text, GLfloat x1, GLfloat y, GLfloat scale, Math::vec3<GLfloat> color)
{
    // Activate corresponding render state	
    //shader.Use();
    Engine::ResourceManager::shaders["textShader"]->use();
    Engine::ResourceManager::shaders["textShader"]->addUniformVec3f("textColor", color, true);
    //glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);

    // Iterate through all characters
    std::string::const_iterator c;
    GLfloat x = x1;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Engine::FontChar ch = *Engine::ResourceManager::font[*c];

        GLfloat xpos = x + ch.bearing.x * scale;
        GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;
        //Logger::w("g_w:" + std::to_string(w) + ", g_h:" + std::to_string(h));
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureId);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance>>6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
