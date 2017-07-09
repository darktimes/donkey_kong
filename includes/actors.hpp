#pragma once

#include "renderer.hpp"
#include <GLFW/glfw3.h>
#include <string>
#include <array>
#include <map>

#include "physEntity.hpp"
#include "collisionSet.hpp"
#include "math.hpp"
#include "animation.hpp"


namespace Physics {
	class CollisionEntity;
}

namespace Engine {
	class Actor {

		public:
			GLfloat width, height;
			static std::array<unsigned, 6> stripIndicies;
			Math::vec2<GLfloat>* position;
			Physics::CollisionEntity* collisionEntity;
			virtual void draw();
			virtual std::string getTextureName() = 0;
			virtual ~Actor();
		private:
//			std::string textureName;


		protected:
			Actor(GLfloat width, GLfloat height, Math::vec2<GLfloat>* position);
	};

	class TerrainBlock: public Actor {

		public:
			enum TerrainType {TerrainBalk, TerrainLadder};

			static const float blockEdgeLength;

			TerrainBlock(TerrainType type, Math::vec2<GLfloat>* position);
			std::string getTextureName() override;
			~TerrainBlock();
		private:
			TerrainType type;

	};

	class AnimatedActor: public Actor {
		public:
			AnimatedActor(GLfloat width, GLfloat height, Math::vec2<GLfloat>* pos);
			std::string getTextureName() override;
			~AnimatedActor();
		private:
			virtual Animation* getCurrentAnimation() = 0;

		protected:
			std::map<std::string, Animation*> animations;
			void addAnimation(Animation* animation);

	};

	class Mario: public AnimatedActor {
		public:
			enum MarioAnimState {RUNNING_LEFT, RUNNING_RIGHT, JUMPING_LEFT, JUMPING_RIGHT, CLIMBING};
			Mario(Math::vec2<GLfloat>* pos);

			MarioAnimState animState;
		private:
			Animation* getCurrentAnimation() override;
	};
}
