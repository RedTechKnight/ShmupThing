#include "../headers/Light.h"

LightManager::LightManager()
{
    for(unsigned int i = 0;i < 64;i++)
    {
        mLights.position[i] = glm::vec4(0,0,0,0);
        mLights.colour[i] = glm::vec4(0,0,0,0);
    }
}

LightManager::~LightManager()
{
    for(unsigned int i = 0;i < 64;i++)
    {
        mLights.position[i] = glm::vec4(0,0,0,0);
        mLights.colour[i] = glm::vec4(0,0,0,0);
    }
}

void LightManager::getLights(std::list<Light>& lights)
{

    for(auto& light: lights)
    {
        if(mLightCount > 63)
        {
            break;
        }

        mLights.position[mLightCount+1] = light.position;
        mLights.colour[mLightCount+1] = light.colour;
        mLightCount++;
    }
    lights.clear();
}

void LightManager::uploadData()
{
    mLights.colour[0].w = mLightCount+1;
    glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(LightBlock),&mLights);
    if(mLightCount > 0)
    {
        std::fill(mLights.position.begin()+1,mLights.position.begin()+mLightCount+1,glm::vec4(0,0,0,0));
        std::fill(mLights.colour.begin()+1,mLights.colour.begin()+mLightCount+1,glm::vec4(0,0,0,0));
        mLightCount = 0;
    }
}
