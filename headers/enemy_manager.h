#ifndef ENEMYMANAGER_H_INCLUDED
#define ENEMYMANAGER_H_INCLUDED
#include "enemy.h"
#include "game_event.h"
#include "gl_particles.h"
#include <iostream>
#include <memory>
struct EnemySpawner {
  int spawnCount;
  unsigned int spawnDelay;
  int nextSpawn;
  glm::vec3 spawnLocation;
  glm::vec3 spawnOffset;
  Enemy enemy;
};

struct ExplosionEvent {
  unsigned int effect_life;
  unsigned int emit_interval;
  unsigned int emit_rate;
  unsigned int max_size;
  unsigned int explosion_sound_effect_id;
  glm::vec3 origin;
  unsigned int ID;
};

class EnemyManager {
private:
  std::vector<unsigned int> mToRemove;

public:
  std::list<ExplosionEvent> explosion_events;
  std::list<HitEvent> hit_events;
  std::list<ProjectileFireEvent> fire_events;

  std::list<Enemy> enemies;
  std::array<glm::vec4, 256> offsets;
  std::array<glm::vec4, 256> hit_flashes;

  glm::vec3 screen_center;
  glm::vec3 player_location;

  float spawn_distance;
  float despawn_distance;

  EnemyManager();
  ~EnemyManager();

  void update(const unsigned int &delta);
};

#endif // ENEMYMANAGER_H_INCLUDED
