#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED
#include <vector>
#include <list>
#include <memory>
#include <map>
#include <queue>
#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <ext/scalar_constants.hpp>
#include <string>
#include <array>
#include <GL/glew.h>
struct LightBlock
{
    std::array<glm::vec4,64> position;
    std::array<glm::vec4,64> colour;
};

struct Light
{
    glm::vec4 position;
    glm::vec4 colour;
};

class LightManager
{
private:
    int mLightCount;
public:
    LightBlock mLights;

    LightManager();
    ~LightManager();

    void getLights(std::list<Light>& lights);
    void uploadData();
};

#endif // LIGHT_H_INCLUDED
