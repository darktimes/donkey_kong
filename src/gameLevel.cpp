#include "gameLevel.hpp"
#include "resourceManager.hpp"
#include "utils.hpp"
#include "renderer.hpp"
#include <iostream>

using namespace Engine;
using namespace Utils;

enum generateDirection {left, right};

Level::Level(unsigned startPointBonus, unsigned xBlockCount, unsigned yBlockCount, IPlayGameStateCallbacks* playGameStateCallbacks):
	startPointBonus(startPointBonus), xBlockCount(xBlockCount), yBlockCount(yBlockCount), playGameStateCallbacks(playGameStateCallbacks) {

}

void Level::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) {

}

Level::~Level() {
	playGameStateCallbacks = nullptr;
	delete mario;
}

void Level::draw() {
        glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ResourceManager::textures["texture_brick"]->id);

	for (TerrainBlock* terrainBlock: terrainBlocks) {
		Math::mat4 model;
		model = Math::scale(model, TerrainBlock::blockEdgeLength);
		model = Math::translate(model, *terrainBlock->position);
		ResourceManager::shaders["projectionShader"]->use();
		ResourceManager::shaders["projectionShader"]->addUniformMatrix4("model", model);
	
		glBindVertexArray(Renderer::SpriteRenderer::sprites["halfedQuadratSprite"]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

}

std::vector<TerrainBlock*> genLevel1Balk(unsigned startHeight, generateDirection direction, unsigned xBlockCount) {
	GLfloat twoThirteenth = TerrainBlock::blockEdgeLength / 13.0f * 2;
	std::vector<TerrainBlock*> vector;
	if (direction == left) {
		int k = 0;
		for (unsigned i = xBlockCount - 2; i  != 0 ; i--) {
			i % 2 == 0 ? k++ : k;
			vector.push_back(new TerrainBlock(TerrainBlock::TerrainBalk, new Math::vec2<GLfloat>((i - 1) * TerrainBlock::blockEdgeLength,
			startHeight * TerrainBlock::blockEdgeLength + k * twoThirteenth)));
		}
	} else {
		int k = 0;
		for (unsigned i = 2; i != xBlockCount; i++) {
			i % 2 == 0 ? k++ : k;
			vector.push_back(new TerrainBlock(TerrainBlock::TerrainBalk, new Math::vec2<GLfloat>(i * TerrainBlock::blockEdgeLength,
			 startHeight * TerrainBlock::blockEdgeLength + k * twoThirteenth)));
		}
	}
	
	return vector;
}

Level1::Level1(IPlayGameStateCallbacks* cbs): Level(2000, 28, 40, cbs) {

	for (unsigned k = 0; k < 14; k++) {
		terrainBlocks.push_back(new TerrainBlock(TerrainBlock::TerrainBalk, new Math::vec2<GLfloat>(k * TerrainBlock::blockEdgeLength, 0.0f)));
	}
	
	GLfloat theSeventh = TerrainBlock::blockEdgeLength / 7.0f;
	unsigned l = 0;
	for (unsigned j = 14; j < 28; j++) {
		j % 2 == 0 ? l++: l;
		Math::vec2<GLfloat>* v = new Math::vec2<GLfloat>((GLfloat)(j * TerrainBlock::blockEdgeLength), (GLfloat)(theSeventh * l));
		terrainBlocks.push_back(new TerrainBlock(TerrainBlock::TerrainBalk, v));
	}

	for (unsigned k = 0; k < 4; k++) {
		std::vector<TerrainBlock*> balkBlocks = genLevel1Balk(3 + k * 4, (k % 2 == 0 ? left : right), 28);
		terrainBlocks.insert(terrainBlocks.end(), balkBlocks.begin(), balkBlocks.end());
	}
	
	for (unsigned k = 0; k < 14; k++) {
		terrainBlocks.push_back(new TerrainBlock(TerrainBlock::TerrainBalk,
		 	new Math::vec2<GLfloat>((GLfloat)(k * TerrainBlock::blockEdgeLength), (GLfloat)(21 * TerrainBlock::blockEdgeLength))));
	}
	
	for (unsigned j = 14; j < 26; j++) {
		terrainBlocks.push_back(new TerrainBlock(TerrainBlock::TerrainBalk,
			new Math::vec2<GLfloat>((GLfloat)(j * TerrainBlock::blockEdgeLength * 1.0f),
				 (GLfloat)(23 * TerrainBlock::blockEdgeLength - theSeventh * (j % 2 == 1 ? j : j+ 1)))));
	}
	
	for (unsigned j = 11; j < 17; j++) {
		terrainBlocks.push_back(new TerrainBlock(TerrainBlock::TerrainBalk,
			new Math::vec2<GLfloat>((GLfloat)(j * TerrainBlock::blockEdgeLength * 1.0f), 25 * TerrainBlock::blockEdgeLength)));
	}
}

void Level1::draw() {
	Level::draw();
}

Level1::~Level1() {
	
}
