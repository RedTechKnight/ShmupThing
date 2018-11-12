#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED
#include <vector>
#include "Enemy.h"
#include "GL_Model.h"
#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <ext/scalar_constants.hpp>
#include <fstream>
#include <istream>
#include <iostream>
struct enemySpawn
{
    glm::vec3 spawnLocation;
    Enemy newEnemy;
};

struct SceneObject
{
    unsigned int modelID;
    glm::vec3 offset;
};

class Level
{
private:

public:
    std::vector<SceneObject> mSceneObjects;

    Level();
    ~Level();

    void addObject(const unsigned int& modelID,const glm::vec3& sceneoffset);
    void loadLevel(const std::string& levelFilename);
};

#endif // LEVEL_H_INCLUDED
