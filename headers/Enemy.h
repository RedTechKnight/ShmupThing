#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED
#include "ship.h"
#include <list>

class Enemy : public Ship {
private:
  unsigned int current_frame;
  float current_frame_percentage;
  int hit_flash_time;

public:
  std::list<ProjectileFireEvent> fire_events;
  std::list<HitEvent> hit_events;

  glm::vec3 final_position;
  std::vector<glm::vec3> position_frames;
  float milli_sec_step;
  bool screen_relative;
  glm::vec3 screen_center;
  glm::vec3 player_location;
  glm::vec3 move_vel;
  unsigned int fire_rate;
  int next_fire;
  bool is_active;
  float hit_flash;
  unsigned int hit_flash_length;

  Enemy();
  ~Enemy();

  virtual void update(const unsigned int &delta);
  virtual void fire();
  void hit();
};

#endif // ENEMY_H_INCLUDED
