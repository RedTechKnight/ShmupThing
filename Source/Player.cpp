#include "Player.h"

Player::Player() : Ship(),mFireDelay(200)
{
    mBoundingBox.center = glm::vec3(mPosition);
    mBoundingBox.size = glm::vec3(2,2,1);
    mHealth = 1;
    mType = Type::Player;

}

Player::~Player()
{

}

void Player::update(const unsigned int& delta)
{
    mBoundingBox.center = mPosition;
    mNextFire -= delta;
}

void Player::fire()
{
    if(mNextFire <= 0)
    {
        boundingBox bb;
        bb.size = glm::vec3(.5,.5,1);
        bb.center = mPosition;
        ProjectileFireEvent proj;
        proj.bounds = bb;
        proj.position = mPosition;
        proj.velocity = glm::vec3(0,0.4f,0);
        proj.type = mType;
        proj.damage = 30;
        proj.modelID = 45;
        proj.soundEffectID = 15;
        mFireEvents.push_back(proj);
        mNextFire = mFireDelay;
    }
}
