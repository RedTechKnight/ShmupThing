#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "ship.h"

class Player : public Ship {
private:
  int next_fire;

public:
  std::list<ProjectileFireEvent> fire_events;
  std::list<HitEvent> hit_events;

  unsigned int fire_delay;
  float hit_flash;
  unsigned int hit_flash_length;

  Player();
  ~Player();

  virtual void update(const unsigned int &delta);
  virtual void fire();
  void hit();
};

#endif // PLAYER_H_INCLUDED
