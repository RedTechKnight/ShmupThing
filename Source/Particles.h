#ifndef PARTICLES_H_INCLUDED
#define PARTICLES_H_INCLUDED

#include <vector>
#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <ext/scalar_constants.hpp>
#include <gtc/random.hpp>
#include <iostream>
#include <math.h>
#include <random>
#include <functional>
#include <list>
#include <algorithm>
#include "GL_Model.h"
#include "GL_Texture.h"
#include "Light.h"
#include <map>

struct Particle
{
    glm::vec3 offset;
    glm::vec3 velocity;
    bool active;
    int life;
};

class ParticleSystem
{
private:
    int mNextEmitTime;
public:
    std::list<Particle> mParticles;
    std::array<glm::vec4,4096> mOffsets;
    glm::vec3 mOrigin;
    unsigned int mMaxSize;
    unsigned int mEmitRate;
    unsigned int mEmitInterval;
    bool mLoop;
    unsigned int mStartLife;
    int mEffectLife;
    std::function<glm::vec3()> mRandomFunction;

    ParticleSystem();
    ~ParticleSystem();

    void create();
    void update(const float& delta);
    void upload();
};

class ParticleRenderer
{
private:

public:
    GLTexture mTexture;
    std::map<unsigned int,ParticleSystem> mParticleSystems;
    std::map<unsigned int,GLModel> mModels;
    std::list<Light> mLights;

    ParticleRenderer();
    ~ParticleRenderer();

    void addParticleEffect(const unsigned int& ID,const ParticleSystem& system);
    void update(const unsigned int& delta);
    void draw();
};

#endif // PARTICLES_H_INCLUDED
