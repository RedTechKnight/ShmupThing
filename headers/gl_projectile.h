#ifndef PROJECTILE_H_INCLUDED
#define PROJECTILE_H_INCLUDED
#include "gl_light.h"
#include "collision.h"
#include <algorithm>
#include <array>
#include <exception>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <list>
#include <vector>

enum class Type { Player, Enemy };

struct Projectile {
  glm::vec3 position;
  glm::vec3 velocity;
  Type type;
  BoundingBox bounds;
  unsigned int model_id;
  unsigned int damage;
  bool hit;
};

struct ProjectileFireEvent {
  glm::vec3 position;
  glm::vec3 velocity;
  Type type;
  BoundingBox bounds;
  unsigned int model_id;
  unsigned int damage;

  unsigned int sound_effect_id;
};

class ProjectileManager {
private:
public:
  std::list<Projectile> projectiles;
  std::list<Light> lights;
  std::array<glm::vec4, 4096> player_projectile_offsets;
  std::array<glm::vec4, 4096> enemy_projectile_offsets;
  int player_offsets;
  int enemy_offsets;
  glm::vec3 screen_center;
  float x_bounds;
  float y_bounds;

  ProjectileManager();
  ~ProjectileManager();

  void update(const unsigned int &delta);
  int collide(const Type &shipType, const BoundingBox &box);
};

#endif // PROJECTILE_H_INCLUDED
