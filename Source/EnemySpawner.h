#ifndef ENEMYSPAWNER_H_INCLUDED
#define ENEMYSPAWNER_H_INCLUDED
#include "Enemy.h"

struct Spawner
{
    glm::vec3 origin;
    glm::vec3 offset;
    float spawned;
    int spawnCount;
    unsigned int spawnInterval;
    int nextSpawn;
    Enemy enemy;
};

struct EnemySpawnEvent
{
    Enemy enemy;
};

class EnemySpawner
{
private:
public:
    std::list<Spawner> mSpawners;
    std::list<EnemySpawnEvent> mSpawnEvents;

    EnemySpawner();
    ~EnemySpawner();

    void update(const unsigned int& delta);
};

#endif // ENEMYSPAWNER_H_INCLUDED
