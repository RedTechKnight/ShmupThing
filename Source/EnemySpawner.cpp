#include "EnemySpawner.h"

EnemySpawner::EnemySpawner()
{
    mSpawners.clear();
}

EnemySpawner::~EnemySpawner()
{
    mSpawners.clear();
}

void EnemySpawner::update(const unsigned int& delta)
{
    for(auto& spawner: mSpawners)
    {

    }
}
