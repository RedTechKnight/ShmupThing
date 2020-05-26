#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>

struct BoundingBox {
  glm::vec3 center;
  glm::vec3 size;
};

bool are_colliding(const BoundingBox &a, const BoundingBox &b);

#endif
