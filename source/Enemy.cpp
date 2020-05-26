#include "../headers/Enemy.h"

Enemy::Enemy() : Ship(),mCurrentFrame(0),mCurrentFramePercentage(0.0f),mHitFlashTime(0),
mMilliSecStep(0.01f),mScreenRelative(false),mScreenCenter(0,0,0),mPlayerLocation(0,0,0),
mFireRate(400),mNextFire(400),mActive(false),mHitFlash(0.0f),mHitFlashLength(300)
{
    mType = Type::Enemy;
    mHealth = 10;
    mPositionFrames.clear();
    mBoundingBox.size = glm::vec3(2,2,1);
}

Enemy::~Enemy()
{
    mPositionFrames.clear();
}

void Enemy::update(const unsigned int& delta)
{
    if(mHitFlash > 0)
    {
        mHitFlash = mHitFlash * (static_cast<float>(mHitFlashTime)/static_cast<float>(mHitFlashLength));
        mHitFlashTime -= delta;
    }
    mNextFire -= delta;
    if(mPositionFrames.size()>0)
    {
        mCurrentFramePercentage += mMilliSecStep * delta;

        if(mCurrentFramePercentage > 1)
        {
            mCurrentFrame++;
            mCurrentFramePercentage -= 1;
        }

        if(mCurrentFrame >= mPositionFrames.size())
        {
            mCurrentFrame = 0;
        }
        if(mPositionFrames.size() > 1)
        {
            if(mCurrentFrame < mPositionFrames.size()-1)
            {
                mPosition = mPositionFrames[mCurrentFrame] + (mPositionFrames[mCurrentFrame+1] - mPositionFrames[mCurrentFrame]) * mCurrentFramePercentage;
            }
            else
            {
                mPosition = mPositionFrames[mCurrentFrame] + (mPositionFrames[0] - mPositionFrames[mCurrentFrame]) * mCurrentFramePercentage;
            }
        }
        else
        {
            mPosition = mPositionFrames[0];
        }
    }

        if(glm::length(mMoveVel) > 0)
        {
            mPosition = mPosition + static_cast<float>(delta) * mMoveVel;
        }
        if(mPosition.x > 55)
        {
            mPosition.x = -54;
        }
        if(mPosition.x < -55)
        {
            mPosition.x = 54;
        }
        mFinalPosition = mPosition;
        if(mScreenRelative)
        {
            mFinalPosition = mPosition + mScreenCenter;
        }
        mBoundingBox.center = mFinalPosition;


            if(mNextFire < 0)
            {
                fire();
                mNextFire = mFireRate;
            }

}


void Enemy::fire()
{
    ProjectileFireEvent proj;
    proj.position = mFinalPosition;
    proj.velocity = glm::normalize(mPlayerLocation - mFinalPosition)*0.01f;
    proj.bounds.center = mFinalPosition;
    proj.bounds.size = glm::vec3(.5,.5,1);
    proj.type = mType;
    proj.modelID = 15;
    proj.damage = 1;
    proj.soundEffectID = 15;
    mFireEvents.push_back(proj);
  /*  glm::mat3 rot = glm::rotate(glm::mat4(1),glm::pi<float>()/7.5f,glm::vec3(0,0,1));
    proj.velocity = proj.velocity * rot;
    proj.modelID = 16;
    mFireEvents.push_back(proj);
    rot = glm::rotate(glm::mat4(1),-glm::pi<float>()/3.25f,glm::vec3(0,0,1));
    proj.velocity = proj.velocity * rot;
    proj.modelID = 17;
    mFireEvents.push_back(proj);*/
}

void Enemy::hit()
{
    mHitFlash = 1.0f;
    mHitFlashTime = mHitFlashLength;
    HitEvent hit;
    hit.hitEffectID = 4;
    mHitEvents.push_back(hit);
}
