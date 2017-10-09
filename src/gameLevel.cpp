#include "game.hpp"
#include "gameLevel.hpp"
#include "utils.hpp"

using namespace Utils;
using namespace Math;
using namespace Rendering;

enum balkDirection {left, right};

Level::Level(unsigned startPointBonus, vec2ui size, vec2f finishAreaPosition,
  vec2f finishAreaDimensions,
  std::string levelName):
		startPointBonus(startPointBonus), size(size),
    finishAreaPosition(finishAreaPosition),
    finishAreaDimensions(finishAreaDimensions),
		levelName(std::move(levelName)) {
  barrelSpawnThreshold = (rand() % 20) / 10.0f;
  barrelSpawnTimer = 0.0f;
	//objView = new SceneView();
}

/**
*  Determines whether the given actor is on the ladder and can climb it
*/
bool Level::canClimb(Actor* actor) {
  for (const auto& ladder: ladders) {
		bool x_collided = actor->position.x + actor->dimensions.x * (7.0f/8.0f) <= ladder->position.x + ladder->dimensions.x && actor->position.x + actor->dimensions.x / 8.0f >= ladder->position.x;
		bool y_collided = actor->position.y + 2.0f >= ladder->position.y && actor->position.y + actor->dimensions.y - 2 <= ladder->position.y + ladder->dimensions.y;
		if (x_collided && y_collided) {
			return true;
		}
	}
  return false;
}

/**
* Generates the inclined balk
*/
std::vector<TerrainBlock*> genLevelOneBalk(unsigned startHeight,
	 balkDirection direction, unsigned xBlockCount) {
	float theSeventh = GameObject::blockEdgeLength / 7.0f;
	std::vector<TerrainBlock*> vector;
	if (direction == left) {
		int k = 0;
		for (unsigned i = xBlockCount - 2; i != 0 ; i--) {
			i % 2 == 0 ? k++ : k;
			vector.push_back(TerrainBlock::genBalkBlock(Math::vec2f((i - 1)
			 * GameObject::blockEdgeLength,
			 startHeight * GameObject::blockEdgeLength + k * theSeventh)));
		}
	} else {
		int k = 0;
		for (unsigned i = 2; i != xBlockCount; i++) {
			i % 2 == 0 ? k++ : k;
			vector.push_back(TerrainBlock::genBalkBlock(Math::vec2f(i *
				GameObject::blockEdgeLength,
			  startHeight * GameObject::blockEdgeLength + k * theSeventh)));
		}
	}

	return vector;
}

/**
* Initializes the level one by adding terrain, ladders and mario
*/
LevelOne::LevelOne(): Level(2000, vec2ui(28, 40),
 vec2f(220.0f, 520.0f), vec2f(100.0f, 30.0f),
 std::string{"Level 1"}) {
	for (unsigned k = 0; k < 14; k++) {
		terrainBlocks.push_back(std::unique_ptr<TerrainBlock>(
		 TerrainBlock::genBalkBlock(Math::vec2f(k *
		 GameObject::blockEdgeLength, 0.0f))));
	}

	float theSeventh = GameObject::blockEdgeLength / 7.0f;
	unsigned l = 0;
	for (unsigned j = 14; j < 28; j++) {
		j % 2 == 0 ? l++: l;
		Math::vec2f v = Math::vec2f((float)(j * GameObject::blockEdgeLength),
		 (float)(theSeventh * l));
		terrainBlocks.push_back(std::unique_ptr<TerrainBlock>((
			TerrainBlock::genBalkBlock(v))));
	}

	for (unsigned k = 0; k < 4; k++) {
		std::vector<TerrainBlock*> tempBalkBlocks =
		 genLevelOneBalk(3 + k * 4, (k % 2 == 0 ? left : right), 28);
		for (auto i : tempBalkBlocks) {
			terrainBlocks.push_back(std::unique_ptr<TerrainBlock>(i));
		}
	}

	for (unsigned k = 0; k < 14; k++) {
		terrainBlocks.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genBalkBlock(
		 	Math::vec2f((float)(k * GameObject::blockEdgeLength),
			(float)(21 * GameObject::blockEdgeLength)))));
	}

	unsigned m = 0;
	for (unsigned j = 14; j < 26; j++) {
		terrainBlocks.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genBalkBlock(
			Math::vec2f((float)(j * GameObject::blockEdgeLength * 1.0f),
				 (float)(21 * GameObject::blockEdgeLength
				 - theSeventh * (j % 2 == 1 ? m : ++m))))));
	}

	for (unsigned j = 11; j < 17; j++) {
		terrainBlocks.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genBalkBlock(
			Math::vec2f((float)(j * GameObject::blockEdgeLength * 1.0f),
			25 * GameObject::blockEdgeLength))));
	}

	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(
    Math::vec2f(GameObject::blockEdgeLength * 10.0f, GameObject::blockEdgeLength / 2.0f),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 1.5f))));

	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 10.0f, GameObject::blockEdgeLength * 3.0f),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 2))));

	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 23.0f, GameObject::blockEdgeLength
		 +  theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 3))));

	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 23.0f, GameObject::blockEdgeLength * 5
		 + 4.0f * theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 4))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 12.0f, GameObject::blockEdgeLength * 4 + 4 * theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 4))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 8.0f, GameObject::blockEdgeLength * 8 +
			theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 4))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 14.0f, GameObject::blockEdgeLength * 9 - 3 * theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 4 + theSeventh))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 24.0f, GameObject::blockEdgeLength * 9 +
			2 * theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 2.5f))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 4.0f, GameObject::blockEdgeLength * 13 +
			theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 3))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 9.0f, GameObject::blockEdgeLength * 12 +
			4 * theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 4))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 24.0f, GameObject::blockEdgeLength * 17 +
			 theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 4))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 21.0f, GameObject::blockEdgeLength * 13 +
			5 * theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 3))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 11.0f, GameObject::blockEdgeLength * 16 +
			2 * theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 2))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 11.0f, GameObject::blockEdgeLength * 19.5f +
			5 * theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 3))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 16.0f, GameObject::blockEdgeLength * 21 +
			1 * theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 5))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 10.0f, GameObject::blockEdgeLength * 21 +
			4 * theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 5))));
  //
	ladders.push_back(std::unique_ptr<TerrainBlock>(TerrainBlock::genLadderBlock(Math::vec2f(
		GameObject::blockEdgeLength * 8.0f, GameObject::blockEdgeLength * 21 +
			4 * theSeventh),
		Math::vec2f(GameObject::blockEdgeLength, GameObject::blockEdgeLength * 5))));

    mario = std::unique_ptr<Mario>(new Mario(vec2f(20.0f, 60.0f)));
}

/**
* Level routine. Place to handle level specific events like spawning barrels
*/
void LevelOne::tick() {
  if (Utils::getTime() - barrelSpawnTimer >= barrelSpawnThreshold) {
    barrelSpawnThreshold  = rand() % 20 / 10.0f;
    barrelSpawnTimer = Utils::getTime();
    barrels.push_back(std::unique_ptr<Barrel>(new Barrel(vec2f(20.0f, 460.0f))));
  }
}
