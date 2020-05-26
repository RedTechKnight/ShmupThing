#include "../headers/EnemyManager.h"
EnemyManager::EnemyManager()
{
    mEnemies.clear();
}

EnemyManager::~EnemyManager()
{
    mEnemies.clear();
}

void EnemyManager::update(const unsigned int& delta)
{


    for(auto& enemy : mEnemies)
    {
        enemy.mScreenCenter = mScreenCenter;
        enemy.mPlayerLocation = mPlayerLocation;

        if(!enemy.mActive)
        {
            if(enemy.mFinalPosition.y < mScreenCenter.y + mSpawnDistance)
            {
                enemy.mActive = true;
            }
        }

        if(enemy.mActive)
        {
            enemy.update(delta);
        }

        if(enemy.mHealth > 0)
        {
            std::insert_iterator< std::list<ProjectileFireEvent> > fireEventInserter(mFireEvents,mFireEvents.end());
            std::move(enemy.mFireEvents.begin(),enemy.mFireEvents.end(),fireEventInserter);
            enemy.mFireEvents.clear();

            std::insert_iterator< std::list<HitEvent> > hitEventInserter(mHitEvents,mHitEvents.end());
            std::move(enemy.mHitEvents.begin(),enemy.mHitEvents.end(),hitEventInserter);
            enemy.mHitEvents.clear();
        }
        else
        {
            ExplosionEvent explosion;
            explosion.explosionSoundEffectID = 3;
            explosion.effectLife = 600;
            explosion.emitInterval = 1;
            explosion.emitRate = 512;
            explosion.maxSize = 512;
            explosion.origin = enemy.mFinalPosition;
            explosion.ID = enemy.mID;
            mExplosionEvents.push_back(explosion);
        }
    }

    auto removeCondition = [](Enemy& enemy) -> bool
    {
        return (enemy.mHealth <= 0);
    };
    mEnemies.erase(std::remove_if(mEnemies.begin(),mEnemies.end(),removeCondition),mEnemies.end());

    auto packOffset = [](Enemy& enemy) -> glm::vec4
    {
        return glm::vec4(enemy.mFinalPosition,1);
    };

    std::transform(mEnemies.begin(),mEnemies.end(),mOffsets.begin(),packOffset);

    auto packHitflash = [](Enemy& enemy) -> glm::vec4
    {
        return glm::vec4(1,1*enemy.mHitFlash,1*enemy.mHitFlash,1);
    };
    std::fill(mHitFlashes.begin(),mHitFlashes.end(),glm::vec4(0,0,0,0));
    std::transform(mEnemies.begin(),mEnemies.end(),mHitFlashes.begin(),packHitflash);
}


