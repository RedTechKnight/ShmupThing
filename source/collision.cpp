#include "../headers/collision.h"

bool are_colliding(const BoundingBox &a, const BoundingBox &b) {
  glm::vec3 center_diff = b.center - a.center;
  if ((a.size.x + b.size.x) / 2.0f > glm::abs(center_diff.x)) {
    if ((a.size.y + b.size.y) / 2.0f > glm::abs(center_diff.y)) {
      if ((a.size.z + b.size.z) / 2.0f > glm::abs(center_diff.z)) {
        return true;
      }
    }
  }
  return false;
}
