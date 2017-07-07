#pragma once

#include <GLFW/glfw3.h>
#include "actors.hpp"
#include "collisionSet.hpp"
#include "math.hpp"

namespace Engine {
	class Actor;
}

namespace Physics {
	
	class PhysEntity {
		public:
			Engine::Actor* actor;
			GLfloat weight;
			Math::vec2<GLfloat> velocity;
			Math::vec2<GLfloat> acceleration;
			bool ged;
	};
};
