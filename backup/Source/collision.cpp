#include "collision.h"

bool collision(const boundingBox& box0,const boundingBox& box1)
{
    glm::vec3 centerDiff = box1.center - box0.center;
    if((box0.size.x + box1.size.x)/2.0f > glm::abs(centerDiff.x))
    {
        if((box0.size.y + box1.size.y)/2.0f > glm::abs(centerDiff.y))
        {
            if((box0.size.z + box1.size.z)/2.0f > glm::abs(centerDiff.z))
            {
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}
