#ifndef ENEMYMANAGER_H_INCLUDED
#define ENEMYMANAGER_H_INCLUDED
#include <memory>
#include <iostream>
#include "Enemy.h"
#include "GameEvent.h"
#include "Particles.h"
struct EnemySpawner
{
    int spawnCount;
    unsigned int spawnDelay;
    int nextSpawn;
    glm::vec3 spawnLocation;
    glm::vec3 spawnOffset;
    Enemy enemy;
};

struct ExplosionEvent
{
    unsigned int effectLife;
    unsigned int emitInterval;
    unsigned int emitRate;
    unsigned int maxSize;
    unsigned int explosionSoundEffectID;
    glm::vec3 origin;
    unsigned int ID;
};

class EnemyManager
{
private:
    std::vector< unsigned int > mToRemove;
public:
    std::list< ExplosionEvent > mExplosionEvents;
    std::list< HitEvent > mHitEvents;
    std::list< ProjectileFireEvent > mFireEvents;

    std::list< Enemy > mEnemies;
    std::array<glm::vec4,256> mOffsets;
    std::array<glm::vec4,256> mHitFlashes;

    glm::vec3 mScreenCenter;
    glm::vec3 mPlayerLocation;

    float mSpawnDistance;
    float mDespawnDistance;

    EnemyManager();
    ~EnemyManager();

    void update(const unsigned int& delta);
};

#endif // ENEMYMANAGER_H_INCLUDED
