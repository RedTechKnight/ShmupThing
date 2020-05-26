#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>

struct boundingBox
{
    glm::vec3 center;
    glm::vec3 size;
};

bool collision(const boundingBox& box0,const boundingBox& box1);


#endif // COLLISION_H_INCLUDED
