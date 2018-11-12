#ifndef RESOURCELOADER_H_INCLUDED
#define RESOURCELOADER_H_INCLUDED
#include <vector>
#include <memory>
#include <map>
#include <queue>
#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <ext/scalar_constants.hpp>
#include <fstream>
#include <istream>
#include <string>
#include "ship.h"
#include "collision.h"
#include "Enemy.h"
#include "EnemyManager.h"
class ResourceLoader
{
private:
public:
    ResourceLoader();
    ~ResourceLoader();

    void loadEnemies(EnemyManager& enemyManager,const std::string& filename);
};


#endif // RESOURCELOADER_H_INCLUDED
