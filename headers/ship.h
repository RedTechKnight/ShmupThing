#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED
#include "game_event.h"
#include "gl_projectile.h"
#include "collision.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

struct HitEvent {
  unsigned int hit_effect_id;
};

class Ship {
public:
  Ship();
  ~Ship();
  glm::vec3 position;
  unsigned int id;
  Type type;
  unsigned int model_id;
  int health;
  BoundingBox bounding_box;

  virtual void fire() = 0;
  virtual void update(const unsigned int &delta) = 0;
};

#endif // SHIP_H_INCLUDED
