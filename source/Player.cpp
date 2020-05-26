#include "../headers/Player.h"

Player::Player() : Ship(), fire_delay(200), hit_flash(0), hit_flash_length(400) {
  bounding_box.center = glm::vec3(position);
  bounding_box.size = glm::vec3(2, 2, 1);
  health = 1;
  type = Type::Player;
}

Player::~Player() {}

void Player::update(const unsigned int &delta) {
  bounding_box.center = position;
  next_fire -= delta;
  if (hit_flash > 0) {
    hit_flash -=
        static_cast<float>(delta) / static_cast<float>(hit_flash_length);
  }
}

void Player::fire() {
  if (next_fire <= 0) {
    BoundingBox bb;
    bb.size = glm::vec3(.5, .5, 1);
    bb.center = position;
    ProjectileFireEvent proj;
    proj.bounds = bb;
    proj.position = position;
    proj.velocity = glm::vec3(0, 0.4f, 0);
    proj.type = type;
    proj.damage = 30;
    proj.model_id = 45;
    proj.sound_effect_id = 15;
    fire_events.push_back(proj);
    next_fire = fire_delay;
  }
}

void Player::hit() {
  hit_flash = 1;
  HitEvent hitevnt;
  hitevnt.hit_effect_id = 5;
  hit_events.push_back(hitevnt);
}
