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
#include "physEntity.hpp"


namespace Physics {
	class PhysEntity;
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

	enum TerrainType {TerrainBalk, TerrainLadder};

	class TerrainBlock: public Actor {

		public:


			static const float blockEdgeLength;

			TerrainBlock(TerrainType type, Math::vec2<GLfloat>* position);
			std::string getTextureName() override;
			~TerrainBlock();
			TerrainType type;

	};

	class AnimatedActor: public Actor {
		public:
			enum ActorState {AT_GROUND, JUMPING, CLIMBING};
			Physics::PhysEntity* pEntity;
			AnimatedActor(GLfloat width, GLfloat height, Math::vec2<GLfloat>* pos);
			std::string getTextureName() override;
			virtual ~AnimatedActor();
			virtual Animation* getCurrentAnimation() = 0;
			virtual void handleAnimationState() = 0;
			enum FaceDirection {FACING_LEFT, FACING_RIGHT};
			FaceDirection face;
			void setState(ActorState state);
			bool atGround;
			bool jumping;
			bool climbing;
		protected:
			std::map<std::string, Animation*> animations;
			void addAnimation(Animation* animation, std::string name);


	};

	class Mario: public AnimatedActor {
		public:

			enum MarioAnimState {ANIM_RUNNING_LEFT, ANIM_RUNNING_RIGHT, ANIM_JUMPING_LEFT, ANIM_JUMPING_RIGHT, ANIM_CLIMBING};

			Mario(Math::vec2<GLfloat>* pos);
			MarioAnimState animState;
			Animation* getCurrentAnimation() override;
			void handleAnimationState() override;
			double lostLifeTimer;

	};

	class Barrel: public AnimatedActor {

		public:
			enum BarrelAnimState {ROLLING, ROLLING_FRONT};
			Barrel(Math::vec2<GLfloat>* pos);
			BarrelAnimState animState;
			void handleAnimationState() override;
			Animation* getCurrentAnimation() override;
			double scoreTimer;
	};
}
