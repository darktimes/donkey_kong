#include "game.hpp"
#include "gameStates.hpp"
#include "resourceManager.hpp"
#include "math.hpp"
#include "utils.hpp"
#include "gameLevel.hpp"
#include "renderer.hpp"
//#include "records.hpp"
#include <cmath>

using namespace Engine;
using namespace Utils;

const std::string MenuGameState::TAG = "MenuGameState";

const std::string PlayGameState::TAG = "PlayGameState";


GameState::GameState() {

}

bool GameState::exits() {
	return glfwWindowShouldClose(Game::getGameWindow()->windowHandle);
}

void GameState::tick() {
	if (!exits()) {
		stateTick();
	}
}


Level* l;



MenuGameState::MenuGameState() : GameState() {

	ResourceManager::shaders["projectionShader"]->addUniformInt("texture1", 0, true);
	ResourceManager::shaders["projectionShader"]->addUniformMatrix4("projection", Math::ortho(0.0f, Game::getGameWindow()->width, 0.0f, Game::getGameWindow()->height, -1.0f, 1.0f), true);
	ResourceManager::shaders["textShader"]->addUniformMatrix4("projection", Math::ortho(0.0f, Game::getGameWindow()->width, 0.0f, Game::getGameWindow()->height, -1.0f, 1.0f), true);
	
	l = Level::generateLevel1(nullptr);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MenuGameState::renderText(std::string text, GLfloat x1, GLfloat y, GLfloat scale, Math::vec3<GLfloat> color)
{
    // Activate corresponding render state	
    //shader.Use();
    ResourceManager::shaders["textShader"]->use();
    ResourceManager::shaders["textShader"]->addUniformVec3f("textColor", color, true);
    //glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    GLfloat x = x1;
    for (c = text.begin(); c != text.end(); c++) 
    {
        FontChar ch = *ResourceManager::font[*c];

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
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

void MenuGameState::stateTick() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ResourceManager::textures["texture_brick"]->id);
	
	for (TerrainBlock* terrainBlock: l->terrainBlocks) {
		Math::mat4 model;
		model = Math::scale(model, TerrainBlock::blockEdgeLength);
		model = Math::translate(model, *terrainBlock->position);
		ResourceManager::shaders["projectionShader"]->use();
		ResourceManager::shaders["projectionShader"]->addUniformMatrix4("model", model);
	
		glBindVertexArray(Renderer::SpriteRenderer::sprites["halfedQuadratSprite"]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	
        glActiveTexture(GL_TEXTURE0);

	renderText("Score", 0.0f, 780.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.0f, 0.0f));
	renderText("Max.Record", 200.0f, 780.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.0f, 0.0f));
	renderText("Bonus", 450.0f, 780.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.0f, 0.0f));
	Record* firstRecord = ResourceManager::recordTable->getRecords()[0];
	renderText(std::string((firstRecord->playerName) + ": " + std::to_string(firstRecord->score)), 200.0f, 760.0f, 0.5f, Math::vec3<GLfloat>(0.5f, 0.5f, 0.0f));

	renderText("0", 0.0f, 760.0f, 0.5f, Math::vec3<GLfloat>(0.0f, 0.0f, 0.5f));
	renderText("3000", 450.0f, 760.0f, 0.5f, Math::vec3<GLfloat>(0.0f, 0.0f, 0.5f));


}

bool MenuGameState::exits() {
	return GameState::exits();
}

std::string MenuGameState::getTag() {
	return TAG;
}

void MenuGameState::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}
		

PlayGameState::PlayGameState() : GameState() {


}

void PlayGameState::stateTick() {
	//draw stuff
}

bool PlayGameState::exits() {
	return GameState::exits();
}

std::string PlayGameState::getTag() {
	return TAG;
}

void PlayGameState::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) {

}


double PlayGameState::getElapsedTime() {
	return glfwGetTime();
}

void PlayGameState::decreaseLife() {


}

void PlayGameState::addPoints(int points) {

}

void PlayGameState::finishLevel() {

}
