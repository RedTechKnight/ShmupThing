#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "ship.h"

class Player : public Ship
{
private:
    int mNextFire;
public:
    std::list< ProjectileFireEvent > mFireEvents;

    unsigned int mFireDelay;

    Player();
    ~Player();

    virtual void update(const unsigned int& delta);
    virtual void fire();
};

#endif // PLAYER_H_INCLUDED
