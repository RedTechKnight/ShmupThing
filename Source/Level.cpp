#include "Level.h"

Level::Level()
{
    mSceneObjects.clear();
}

Level::~Level()
{
    mSceneObjects.clear();
}

void Level::addObject(const unsigned int& modelID,const glm::vec3& offset)
{
    SceneObject object;
    object.modelID = modelID;
    object.offset = offset;
    mSceneObjects.push_back(object);
}

void Level::loadLevel(const std::string& filename)
{
    std::ifstream infile(filename,std::ios::in);

    while(infile.good())
    {
        SceneObject object;
        std::string value;
        infile >> value;
        if(value == "object")
        {
            infile >> object.modelID >> object.offset.x >> object.offset.y >> object.offset.z;
        }
        mSceneObjects.push_back(object);
    }
}
