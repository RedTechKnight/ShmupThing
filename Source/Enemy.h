#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED
#include <list>
#include "ship.h"

struct HitEvent
{
    unsigned int hitEffectID;
};

class Enemy : public Ship
{
private:
    unsigned int mCurrentFrame;
    float mCurrentFramePercentage;
    int mHitFlashTime;
public:
    std::list< ProjectileFireEvent > mFireEvents;
    std::list< HitEvent > mHitEvents;

    glm::vec3 mFinalPosition;
    std::vector<glm::vec3> mPositionFrames;
    float mMilliSecStep;
    bool mScreenRelative;
    glm::vec3 mScreenCenter;
    glm::vec3 mPlayerLocation;
    glm::vec3 mMoveVel;
    unsigned int mFireRate;
    int mNextFire;
    bool mActive;
    float mHitFlash;
    unsigned int mHitFlashLength;

    Enemy();
    ~Enemy();

    virtual void update(const unsigned int& delta);
    virtual void fire();
    void hit();
};



#endif // ENEMY_H_INCLUDED
