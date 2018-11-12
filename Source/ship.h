#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED
#include <vector>
#include <memory>
#include <map>
#include <queue>
#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <ext/scalar_constants.hpp>
#include <string>
#include "collision.h"
#include "Projectile.h"
#include "GameEvent.h"

class Ship
{
public:
    Ship();
    ~Ship();
    glm::vec3 mPosition;
    unsigned int mID;
    Type mType;
    unsigned int mModelID;
    int mHealth;
    boundingBox mBoundingBox;

    virtual void fire() = 0;
    virtual void update(const unsigned int& delta) = 0;
};

#endif // SHIP_H_INCLUDED
