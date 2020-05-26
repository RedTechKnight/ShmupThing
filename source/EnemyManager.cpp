#include "../headers/EnemyManager.h"
EnemyManager::EnemyManager() { enemies.clear(); }

EnemyManager::~EnemyManager() { enemies.clear(); }

void EnemyManager::update(const unsigned int &delta) {

  for (auto &enemy : enemies) {
    enemy.screen_center = screen_center;
    enemy.player_location = player_location;

    if (!enemy.is_active) {
      if (enemy.final_position.y < screen_center.y + spawn_distance) {
        enemy.is_active = true;
      }
    }

    if (enemy.is_active) {
      enemy.update(delta);
    }

    if (enemy.health > 0) {
      std::insert_iterator<std::list<ProjectileFireEvent>> fire_event_inserter(
          fire_events, fire_events.end());
      std::move(enemy.fire_events.begin(), enemy.fire_events.end(),
                fire_event_inserter);
      enemy.fire_events.clear();

      std::insert_iterator<std::list<HitEvent>> hit_event_inserter(
          hit_events, hit_events.end());
      std::move(enemy.hit_events.begin(), enemy.hit_events.end(),
                hit_event_inserter);
      enemy.hit_events.clear();
    } else {
      ExplosionEvent explosion;
      explosion.explosion_sound_effect_id = 3;
      explosion.effect_life = 600;
      explosion.emit_interval = 1;
      explosion.emit_rate = 512;
      explosion.max_size = 512;
      explosion.origin = enemy.final_position;
      explosion.ID = enemy.id;
      explosion_events.push_back(explosion);
    }
  }

  auto remove_condition = [](Enemy &enemy) -> bool {
    return (enemy.health <= 0);
  };
  enemies.erase(
      std::remove_if(enemies.begin(), enemies.end(), remove_condition),
      enemies.end());

  auto pack_offset = [](Enemy &enemy) -> glm::vec4 {
    return glm::vec4(enemy.final_position, 1);
  };

  std::transform(enemies.begin(), enemies.end(), offsets.begin(),
                 pack_offset);

  auto pack_hitflash = [](Enemy &enemy) -> glm::vec4 {
    return glm::vec4(1, 1 * enemy.hit_flash, 1 * enemy.hit_flash, 1);
  };
  std::fill(hit_flashes.begin(), hit_flashes.end(), glm::vec4(0, 0, 0, 0));
  std::transform(enemies.begin(), enemies.end(), hit_flashes.begin(),
                 pack_hitflash);
}
