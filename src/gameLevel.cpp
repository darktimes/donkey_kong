#include "gameLevel.hpp"
#include "utils.hpp"
#include <iostream>

using namespace Engine;
using namespace Utils;

enum generateDirection {left, right};

Level::Level(unsigned startPointBonus, unsigned xBlockCount, unsigned yBlockCount, IPlayGameStateCallbacks* playGameStateCallbacks):
	startPointBonus(startPointBonus), xBlockCount(xBlockCount), yBlockCount(yBlockCount), playGameStateCallbacks(playGameStateCallbacks) {

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

Level* Level::generateLevel1(IPlayGameStateCallbacks* playGameStateCallbacks) {
	
	Level* level = new Level(2000, 28, 40, playGameStateCallbacks);
	for (unsigned k = 0; k < 14; k++) {
		level->terrainBlocks.push_back(new TerrainBlock(TerrainBlock::TerrainBalk,
		 	new Math::vec2<GLfloat>(k * TerrainBlock::blockEdgeLength, 0.0f)));
	}
	

	GLfloat theSeventh = TerrainBlock::blockEdgeLength / 7.0f;
	unsigned l = 0;
	for (unsigned j = 14; j < 28; j++) {
		j % 2 == 0 ? l++: l;
		Math::vec2<GLfloat>* v = new Math::vec2<GLfloat>((GLfloat)(j * TerrainBlock::blockEdgeLength), (GLfloat)(theSeventh * l));
		level->terrainBlocks.push_back(new TerrainBlock(TerrainBlock::TerrainBalk, v));
	}

	for (unsigned k = 0; k < 4; k++) {
		std::vector<TerrainBlock*> balkBlocks = genLevel1Balk(3 + k * 4, (k % 2 == 0 ? left : right), 28);
		level->terrainBlocks.insert(level->terrainBlocks.end(), balkBlocks.begin(), balkBlocks.end());
	}
	
	for (unsigned k = 0; k < 14; k++) {
		level->terrainBlocks.push_back(new TerrainBlock(TerrainBlock::TerrainBalk,
		 	new Math::vec2<GLfloat>((GLfloat)(k * TerrainBlock::blockEdgeLength), (GLfloat)(21 * TerrainBlock::blockEdgeLength))));
	}
	
	for (unsigned j = 14; j < 26; j++) {
		level->terrainBlocks.push_back(new TerrainBlock(TerrainBlock::TerrainBalk,
			new Math::vec2<GLfloat>((GLfloat)(j * TerrainBlock::blockEdgeLength * 1.0f),
				 (GLfloat)(23 * TerrainBlock::blockEdgeLength - theSeventh * (j % 2 == 1 ? j : j+ 1)))));
	}
	
	for (unsigned j = 11; j < 17; j++) {
		level->terrainBlocks.push_back(new TerrainBlock(TerrainBlock::TerrainBalk,
			new Math::vec2<GLfloat>((GLfloat)(j * TerrainBlock::blockEdgeLength * 1.0f), 25 * TerrainBlock::blockEdgeLength)));
	} 

	return level;
}

void Level::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) {

}

Level::~Level() {
	playGameStateCallbacks = nullptr;
	delete mario;
}

void Level::draw() {
	
}


