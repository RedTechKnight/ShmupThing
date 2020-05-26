#include "../headers/Projectile.h"

ProjectileManager::ProjectileManager()
{
    mProjectiles.clear();
}

ProjectileManager::~ProjectileManager()
{
    mProjectiles.clear();
}

void ProjectileManager::update(const unsigned int& delta)
{
    std::fill(mPlayerProjectileOffsets.begin(),mPlayerProjectileOffsets.end(),glm::vec4(0,0,0,0));
    std::fill(mEnemyProjectileOffsets.begin(),mEnemyProjectileOffsets.end(),glm::vec4(0,0,0,0));
    auto func = [&delta](projectile& iter) -> void
    {
        iter.position = iter.position + (iter.velocity * static_cast<float>(delta));
        iter.bounds.center = iter.position;
    };

    std::for_each(mProjectiles.begin(),mProjectiles.end(),func);

    float xbounds = mXBounds;
    float ybounds = mYBounds;
    glm::vec3 center = mScreenCenter;

    auto outOfBounds = [xbounds,ybounds,center](projectile& iter) -> bool
    {
        if(iter.position.x > center.x + xbounds)
        {
            return true;
        }
        else if(iter.position.x < center.x - xbounds)
        {
            return true;
        }
        else if(iter.position.y > center.y + ybounds)
        {
            return true;
        }
        else if(iter.position.y < center.y - ybounds)
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    mProjectiles.erase(std::remove_if(mProjectiles.begin(),mProjectiles.end(),outOfBounds),mProjectiles.end());

    mPlayerOffsets = 0;
    mEnemyOffsets = 0;
    if(mProjectiles.size() > 0)
    {
        for(auto& proj: mProjectiles)
        {
            if(proj.type == Type::Player)
            {
                mPlayerProjectileOffsets[mPlayerOffsets] = glm::vec4(proj.position,1);
                mPlayerOffsets++;
                Light l;
                l.position = glm::vec4(proj.position,10);
                l.colour = glm::vec4(0,0.7,0,1);
                mLights.push_back(l);
            }
            else if(proj.type == Type::Enemy)
            {
                mEnemyProjectileOffsets[mEnemyOffsets] = glm::vec4(proj.position,1);
                mEnemyOffsets++;
            }
        }


    }
}

int ProjectileManager::collide(const Type& shipType,const boundingBox& box)
{
    int totalDamage = 0;
    auto collisionTest = [shipType,box,&totalDamage](projectile& proj) -> void
    {
        if(shipType != proj.type)
        {
            if(collision(box,proj.bounds))
            {
                totalDamage += proj.damage;
                proj.hit = true;
            }
        }
    };
    
    std::for_each(mProjectiles.begin(),mProjectiles.end(),collisionTest);

    auto removeCond = [](projectile& proj) -> bool
    {
        return proj.hit;
    };

    mProjectiles.erase(std::remove_if(mProjectiles.begin(),mProjectiles.end(),removeCond) ,mProjectiles.end());

    return totalDamage;
}
