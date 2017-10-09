#pragma once

#include "math.hpp"

namespace Physics {
  /**
  * Contains relevant physical information. It is stored by an Actor.
  */
  class PhysEntity {
    public:
      PhysEntity(float weight);
      ~PhysEntity() {};
      float weight;
      Math::vec2f velocity;
      Math::vec2f acceleration;
  };
}
