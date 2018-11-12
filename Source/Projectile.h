#ifndef PROJECTILE_H_INCLUDED
#define PROJECTILE_H_INCLUDED
#include <vector>
#include <list>
#include <array>
#include <exception>
#include <algorithm>
#include <iostream>
#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <ext/scalar_constants.hpp>
#include "collision.h"
#include "Light.h"

enum class Type
{
    Player,
    Enemy
};

struct projectile
{
    glm::vec3 position;
    glm::vec3 velocity;
    Type type;
    boundingBox bounds;
    unsigned int modelID;
    unsigned int damage;
    bool hit;
};

struct ProjectileFireEvent
{
    glm::vec3 position;
    glm::vec3 velocity;
    Type type;
    boundingBox bounds;
    unsigned int modelID;
    unsigned int damage;

    unsigned int soundEffectID;
};

class ProjectileManager
{
private:

public:
    std::list<projectile> mProjectiles;
    std::list<Light> mLights;
    std::array<glm::vec4,4096> mPlayerProjectileOffsets;
    std::array<glm::vec4,4096> mEnemyProjectileOffsets;
    int mPlayerOffsets;
    int mEnemyOffsets;
    glm::vec3 mScreenCenter;
    float mXBounds;
    float mYBounds;

    ProjectileManager();
    ~ProjectileManager();

    void update(const unsigned int& delta);
    int collide(const Type& shipType,const boundingBox& box);
};

#endif // PROJECTILE_H_INCLUDED
