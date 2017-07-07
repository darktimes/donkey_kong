#pragma once

#include "renderer.hpp"
#include <GLFW/glfw3.h>
#include <string>
#include <array>


#include "physEntity.hpp"
#include "collisionSet.hpp"
#include "math.hpp"


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

			virtual ~Actor();
		private:
			std::string textureName;
			
			
		protected:
			Actor(GLfloat width, GLfloat height, std::string textureName, Math::vec2<GLfloat>* position);
	};
	
	class TerrainBlock: public Actor {

		public:
			enum TerrainType {TerrainBalk, TerrainLadder};
			
			static const float blockEdgeLength;
			TerrainBlock(TerrainType type, Math::vec2<GLfloat>* position);
			~TerrainBlock();
			
	};
	
	class AnimatedActor: public Actor {
	
	};
	
	class Mario: public AnimatedActor {
		
	};
}
