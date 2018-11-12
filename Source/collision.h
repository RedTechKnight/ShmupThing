#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <ext/scalar_constants.hpp>

struct boundingBox
{
    glm::vec3 center;
    glm::vec3 size;
};

bool collision(const boundingBox& box0,const boundingBox& box1);


#endif // COLLISION_H_INCLUDED
