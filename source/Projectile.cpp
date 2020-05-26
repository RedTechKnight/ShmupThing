#include "../headers/Projectile.h"

ProjectileManager::ProjectileManager() { projectiles.clear(); }

ProjectileManager::~ProjectileManager() { projectiles.clear(); }

void ProjectileManager::update(const unsigned int &delta) {
  std::fill(player_projectile_offsets.begin(), player_projectile_offsets.end(),
            glm::vec4(0, 0, 0, 0));
  std::fill(enemy_projectile_offsets.begin(), enemy_projectile_offsets.end(),
            glm::vec4(0, 0, 0, 0));
  auto func = [&delta](Projectile &iter) -> void {
    iter.position = iter.position + (iter.velocity * static_cast<float>(delta));
    iter.bounds.center = iter.position;
  };

  std::for_each(projectiles.begin(), projectiles.end(), func);

  float xbounds = x_bounds;
  float ybounds = y_bounds;
  glm::vec3 center = screen_center;

  auto out_of_bounds = [xbounds, ybounds, center](Projectile &iter) -> bool {
    if (iter.position.x > center.x + xbounds) {
      return true;
    } else if (iter.position.x < center.x - xbounds) {
      return true;
    } else if (iter.position.y > center.y + ybounds) {
      return true;
    } else if (iter.position.y < center.y - ybounds) {
      return true;
    } else {
      return false;
    }
  };
  projectiles.erase(
      std::remove_if(projectiles.begin(), projectiles.end(), out_of_bounds),
      projectiles.end());

  player_offsets = 0;
  enemy_offsets = 0;
  if (projectiles.size() > 0) {
    for (auto &proj : projectiles) {
      if (proj.type == Type::Player) {
        player_projectile_offsets[player_offsets] = glm::vec4(proj.position, 1);
        player_offsets++;
        Light l;
        l.position = glm::vec4(proj.position, 10);
        l.colour = glm::vec4(0, 0.7, 0, 1);
        lights.push_back(l);
      } else if (proj.type == Type::Enemy) {
        enemy_projectile_offsets[enemy_offsets] = glm::vec4(proj.position, 1);
        enemy_offsets++;
      }
    }
  }
}

int ProjectileManager::collide(const Type &ship_type, const BoundingBox &box) {
  int total_damage = 0;
  auto collision_test = [ship_type, box,
                         &total_damage](Projectile &proj) -> void {
    if (ship_type != proj.type) {
      if (are_colliding(box, proj.bounds)) {
        total_damage += proj.damage;
        proj.hit = true;
      }
    }
  };

  std::for_each(projectiles.begin(), projectiles.end(), collision_test);

  auto remove_cond = [](Projectile &proj) -> bool { return proj.hit; };

  projectiles.erase(
      std::remove_if(projectiles.begin(), projectiles.end(), remove_cond),
      projectiles.end());

  return total_damage;
}
