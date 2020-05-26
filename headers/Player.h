#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "ship.h"

class Player : public Ship
{
private:
    int mNextFire;
public:
    std::list< ProjectileFireEvent > mFireEvents;
    std::list< HitEvent > mHitEvents;

    unsigned int mFireDelay;
    float mHitFlash;
    unsigned int mHitFlashLength;

    Player();
    ~Player();

    virtual void update(const unsigned int& delta);
    virtual void fire();
    void hit();
};

#endif // PLAYER_H_INCLUDED
