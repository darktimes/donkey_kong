#include "gameLevel.hpp"
#include "resourceManager.hpp"
#include "utils.hpp"
#include "renderer.hpp"
#include <iostream>

using namespace Engine;
using namespace Utils;

enum generateDirection {left, right};

Ladder::Ladder(Math::vec2<GLfloat> loc, Math::vec2<GLfloat> dims): location(loc), dimensions(dims) {

}

Level::Level(unsigned startPointBonus, unsigned xBlockCount, unsigned yBlockCount, IPlayGameStateCallbacks* playGameStateCallbacks):
	startPointBonus(startPointBonus), xBlockCount(xBlockCount), yBlockCount(yBlockCount), playGameStateCallbacks(playGameStateCallbacks) {
}

void Level::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) {

}

Level::~Level() {
	playGameStateCallbacks = nullptr;
	delete mario;
}


bool Level::canClimb(AnimatedActor* actor) {
	return true;
}

void Level::draw() {

}

std::vector<TerrainBlock*> genLevel1Balk(unsigned startHeight, generateDirection direction, unsigned xBlockCount) {
	GLfloat theSeventh = TerrainBlock::blockEdgeLength / 7.0f;
	std::vector<TerrainBlock*> vector;
	if (direction == left) {
		int k = 0;
		for (unsigned i = xBlockCount - 2; i  != 0 ; i--) {
			i % 2 == 0 ? k++ : k;
			vector.push_back(new TerrainBlock(TerrainBalk, new Math::vec2<GLfloat>((i - 1) * TerrainBlock::blockEdgeLength,
			startHeight * TerrainBlock::blockEdgeLength + k * theSeventh)));
		}
	} else {
		int k = 0;
		for (unsigned i = 2; i != xBlockCount; i++) {
			i % 2 == 0 ? k++ : k;
			vector.push_back(new TerrainBlock(TerrainBalk, new Math::vec2<GLfloat>(i * TerrainBlock::blockEdgeLength,
			 startHeight * TerrainBlock::blockEdgeLength + k * theSeventh)));
		}
	}

	return vector;
}


void Level1::initMario() {
	mario = new Mario(new Math::vec2<GLfloat>(20.0f, 60.0f));
}


Level1::Level1(IPlayGameStateCallbacks* cbs): Level(2000, 28, 40, cbs) {

	for (unsigned k = 0; k < 14; k++) {
		terrainBlocks.push_back(new TerrainBlock(TerrainBalk, new Math::vec2<GLfloat>(k * TerrainBlock::blockEdgeLength, 0.0f)));
	}

	GLfloat theSeventh = TerrainBlock::blockEdgeLength / 7.0f;
	unsigned l = 0;
	for (unsigned j = 14; j < 28; j++) {
		j % 2 == 0 ? l++: l;
		Math::vec2<GLfloat>* v = new Math::vec2<GLfloat>((GLfloat)(j * TerrainBlock::blockEdgeLength), (GLfloat)(theSeventh * l));
		terrainBlocks.push_back(new TerrainBlock(TerrainBalk, v));
	}

	for (unsigned k = 0; k < 4; k++) {
		std::vector<TerrainBlock*> tempBalkBlocks = genLevel1Balk(3 + k * 4, (k % 2 == 0 ? left : right), 28);
		terrainBlocks.insert(terrainBlocks.end(), tempBalkBlocks.begin(), tempBalkBlocks.end());
	}

	for (unsigned k = 0; k < 14; k++) {
		terrainBlocks.push_back(new TerrainBlock(TerrainBalk,
		 	new Math::vec2<GLfloat>((GLfloat)(k * TerrainBlock::blockEdgeLength), (GLfloat)(21 * TerrainBlock::blockEdgeLength))));
	}

	unsigned m = 0;
	for (unsigned j = 14; j < 26; j++) {
		terrainBlocks.push_back(new TerrainBlock(TerrainBalk,
			new Math::vec2<GLfloat>((GLfloat)(j * TerrainBlock::blockEdgeLength * 1.0f),
				 (GLfloat)(21 * TerrainBlock::blockEdgeLength - theSeventh * (j % 2 == 1 ? m : ++m)))));
	}

	for (unsigned j = 11; j < 17; j++) {
		terrainBlocks.push_back(new TerrainBlock(TerrainBalk,
			new Math::vec2<GLfloat>((GLfloat)(j * TerrainBlock::blockEdgeLength * 1.0f), 25 * TerrainBlock::blockEdgeLength)));
	}

	GLfloat seventh = TerrainBlock::blockEdgeLength / 7.0f;

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength * 2 )));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 2)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength * 3.5 )));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength * 4.5 )));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength * 5.5 )));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength * 3.5),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 3)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 23.0f, TerrainBlock::blockEdgeLength + 5.0f * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 23.0f, TerrainBlock::blockEdgeLength * 2 + 5.0f * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 23.0f, TerrainBlock::blockEdgeLength * 3 + 5.0f * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 23.0f, TerrainBlock::blockEdgeLength * 4 + 5.0f * seventh)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 23.0f, TerrainBlock::blockEdgeLength + 5.0f * seventh),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 4)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 4.0f, TerrainBlock::blockEdgeLength * 5 + 4.0f * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 4.0f, TerrainBlock::blockEdgeLength * 6 + 4.0f * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 4.0f, TerrainBlock::blockEdgeLength * 7 + 4.0f * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 4.0f, TerrainBlock::blockEdgeLength * 8 + 4.0f * seventh)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 4.0f, TerrainBlock::blockEdgeLength * 5 + 4.0f * seventh),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 4)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 12.0f, TerrainBlock::blockEdgeLength * 5)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 12.0f, TerrainBlock::blockEdgeLength * 6)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 12.0f, TerrainBlock::blockEdgeLength * 7)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 12.0f, TerrainBlock::blockEdgeLength * 8)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 12.0f, TerrainBlock::blockEdgeLength * 9)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 12.0f, TerrainBlock::blockEdgeLength * 5),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 5)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 8.0f, TerrainBlock::blockEdgeLength * 8 + 4 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 8.0f, TerrainBlock::blockEdgeLength * 9 + 4 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 8.0f, TerrainBlock::blockEdgeLength * 11 + 4 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 8.0f, TerrainBlock::blockEdgeLength * 12 + 4 * seventh)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 8.0f, TerrainBlock::blockEdgeLength * 8 + 4 * seventh),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 4)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 14.0f, TerrainBlock::blockEdgeLength * 9 )));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 14.0f, TerrainBlock::blockEdgeLength * 10)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 14.0f, TerrainBlock::blockEdgeLength * 11)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 14.0f, TerrainBlock::blockEdgeLength * 12)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 14.0f, TerrainBlock::blockEdgeLength * 13)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 14.0f, TerrainBlock::blockEdgeLength * 9),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 5)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 23.0f, TerrainBlock::blockEdgeLength * 9 + 4.0f * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 23.0f, TerrainBlock::blockEdgeLength * 10 + 4.0f * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 23.0f, TerrainBlock::blockEdgeLength * 11 + 4.0f * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 23.0f, TerrainBlock::blockEdgeLength * 12 + 4.0f * seventh)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 23.0f, TerrainBlock::blockEdgeLength * 9 + 4.0f * seventh),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 4)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 4.0f, TerrainBlock::blockEdgeLength * 13 + 4 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 4.0f, TerrainBlock::blockEdgeLength * 14 + 4 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 4.0f, TerrainBlock::blockEdgeLength * 15 + 4 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 4.0f, TerrainBlock::blockEdgeLength * 16 + 4 * seventh)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 4.0f, TerrainBlock::blockEdgeLength * 13 + 4.0f * seventh),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 4)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 9.0f, TerrainBlock::blockEdgeLength * 13 + 2 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 9.0f, TerrainBlock::blockEdgeLength * 14 + 2 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 9.0f, TerrainBlock::blockEdgeLength * 15 + 2 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 9.0f, TerrainBlock::blockEdgeLength * 16 + 2 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 9.0f, TerrainBlock::blockEdgeLength * 17 + 2 * seventh)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 9.0f, TerrainBlock::blockEdgeLength * 13 + 2.0f * seventh),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 5)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 24.0f, TerrainBlock::blockEdgeLength * 17 + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 24.0f, TerrainBlock::blockEdgeLength * 18 + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 24.0f, TerrainBlock::blockEdgeLength * 19 + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 24.0f, TerrainBlock::blockEdgeLength * 20 + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 24.0f, TerrainBlock::blockEdgeLength * 21 + 5 * seventh)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 24.0f, TerrainBlock::blockEdgeLength * 17 + 5.0f * seventh),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 5)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 21.0f, TerrainBlock::blockEdgeLength * 15 + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 21.0f, TerrainBlock::blockEdgeLength * 16 + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 21.0f, TerrainBlock::blockEdgeLength * 17 + 5 * seventh)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 21.0f, TerrainBlock::blockEdgeLength * 15 + 5.0f * seventh),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 3)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 11.0f, TerrainBlock::blockEdgeLength * 16  + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 11.0f, TerrainBlock::blockEdgeLength * 17  + 5 * seventh)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 11.0f, TerrainBlock::blockEdgeLength * 16 + 5.0f * seventh),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 2)
			)
	);
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 11.0f, TerrainBlock::blockEdgeLength * 19.5  + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 11.0f, TerrainBlock::blockEdgeLength * 20.5  + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 11.0f, TerrainBlock::blockEdgeLength * 21.5  + 5 * seventh)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 11.0f, TerrainBlock::blockEdgeLength * 19.5 + 5.0f * seventh),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 3)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 16.0f, TerrainBlock::blockEdgeLength * 21  + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 16.0f, TerrainBlock::blockEdgeLength * 22  + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 16.0f, TerrainBlock::blockEdgeLength * 23  + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 16.0f, TerrainBlock::blockEdgeLength * 24  + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 16.0f, TerrainBlock::blockEdgeLength * 25  + 5 * seventh)));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 16.0f, TerrainBlock::blockEdgeLength * 26  + 5 * seventh)));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 16.0f, TerrainBlock::blockEdgeLength * 21 + 4.0f * seventh),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 6)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength * 22 )));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength * 23 )));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength * 24 )));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength * 25 )));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength * 26 )));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 10.0f, TerrainBlock::blockEdgeLength * 22),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 5)
			)
	);

	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 8.0f, TerrainBlock::blockEdgeLength * 22 )));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 8.0f, TerrainBlock::blockEdgeLength * 23 )));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 8.0f, TerrainBlock::blockEdgeLength * 24 )));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 8.0f, TerrainBlock::blockEdgeLength * 25 )));
	terrainBlocks.push_back(new TerrainBlock(TerrainLadder, new Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 8.0f, TerrainBlock::blockEdgeLength * 26 )));
	ladders.push_back(
		new Ladder(
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength * 8.0f, TerrainBlock::blockEdgeLength * 22),
			Math::vec2<GLfloat>(TerrainBlock::blockEdgeLength, TerrainBlock::blockEdgeLength * 5)
			)
	);

	initMario();
}

void Level1::draw() {
	Level::draw();
}

Level1::~Level1() {

}
