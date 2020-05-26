#include "../headers/enemy.h"

Enemy::Enemy()
    : Ship(), current_frame(0), current_frame_percentage(0.0f),
      hit_flash_time(0), milli_sec_step(0.01f), screen_relative(false),
      screen_center(0, 0, 0), player_location(0, 0, 0), fire_rate(400),
      next_fire(400), is_active(false), hit_flash(0.0f), hit_flash_length(300) {
  type = Type::Enemy;
  health = 10;
  position_frames.clear();
  bounding_box.size = glm::vec3(2, 2, 1);
}

Enemy::~Enemy() { position_frames.clear(); }

void Enemy::update(const unsigned int &delta) {
  if (hit_flash > 0) {
    hit_flash = hit_flash * (static_cast<float>(hit_flash_time) /
                             static_cast<float>(hit_flash_length));
    hit_flash_time -= delta;
  }
  next_fire -= delta;
  if (position_frames.size() > 0) {
    current_frame_percentage += milli_sec_step * delta;

    if (current_frame_percentage > 1) {
      current_frame++;
      current_frame_percentage -= 1;
    }

    if (current_frame >= position_frames.size()) {
      current_frame = 0;
    }
    if (position_frames.size() > 1) {
      if (current_frame < position_frames.size() - 1) {
        position = position_frames[current_frame] +
                   (position_frames[current_frame + 1] -
                    position_frames[current_frame]) *
                       current_frame_percentage;
      } else {
        position = position_frames[current_frame] +
                   (position_frames[0] - position_frames[current_frame]) *
                       current_frame_percentage;
      }
    } else {
      position = position_frames[0];
    }
  }

  if (glm::length(move_vel) > 0) {
    position = position + static_cast<float>(delta) * move_vel;
  }
  if (position.x > 55) {
    position.x = -54;
  }
  if (position.x < -55) {
    position.x = 54;
  }
  final_position = position;
  if (screen_relative) {
    final_position = position + screen_center;
  }
  bounding_box.center = final_position;

  if (next_fire < 0) {
    fire();
    next_fire = fire_rate;
  }
}

void Enemy::fire() {
  ProjectileFireEvent proj;
  proj.position = final_position;
  proj.velocity = glm::normalize(player_location - final_position) * 0.01f;
  proj.bounds.center = final_position;
  proj.bounds.size = glm::vec3(.5, .5, 1);
  proj.type = type;
  proj.model_id = 15;
  proj.damage = 1;
  proj.sound_effect_id = 15;
  fire_events.push_back(proj);
}

void Enemy::hit() {
  hit_flash = 1.0f;
  hit_flash_time = hit_flash_length;
  HitEvent hit;
  hit.hit_effect_id = 4;
  hit_events.push_back(hit);
}
