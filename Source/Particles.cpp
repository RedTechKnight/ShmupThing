#include "Particles.h"

ParticleSystem::ParticleSystem() : mNextEmitTime(0),mOrigin(0,0,0),mMaxSize(0),mEmitRate(0),mEmitInterval(0),
                                    mLoop(false),mStartLife(0)
{
    mParticles.clear();
}

ParticleSystem::~ParticleSystem()
{
    mParticles.clear();
}
void ParticleSystem::create()
{
    std::fill(mOffsets.begin(),mOffsets.end(),glm::vec4(0,0,0,0));
    if(mEmitRate > mMaxSize)
        mEmitRate = mMaxSize;

    for(unsigned int i = 0;i < mEmitRate;i++)
    {
        Particle particle;
        particle.active = true;
        particle.life = mStartLife;
        particle.offset = mOrigin;
        particle.velocity = mRandomFunction();
        mParticles.push_back(particle);
    }
}

void ParticleSystem::update(const float& delta)
{
    mEffectLife -= delta;

    std::fill(mOffsets.begin(),mOffsets.end(),glm::vec4(0,0,0,0));
    mNextEmitTime -= delta;
    if(mNextEmitTime <= 0)
    {
        mNextEmitTime = mEmitInterval;
        for(unsigned int i = 0;i < glm::min(mMaxSize - static_cast<unsigned int>(mParticles.size()),mEmitRate);i++)
        {
            Particle particle;
            particle.active = true;
            particle.life = mStartLife;
            particle.offset = mOrigin;
            particle.velocity = mRandomFunction();
            mParticles.push_back(particle);
        }
    }

    auto updateOffset = [&delta](Particle& particle) -> void
    {
        particle.offset = particle.offset + particle.velocity * delta/1000.0f;
        particle.life -= delta;
    };

    std::for_each(mParticles.begin(),mParticles.end(),updateOffset);

    auto removeCondition = [](Particle& particle) -> bool
    {
        return (particle.life <= 0);
    };

    mParticles.erase(std::remove_if(mParticles.begin(),mParticles.end(),removeCondition),mParticles.end());

    float start = mStartLife;
    auto offsetPack = [&start](Particle& particle) -> glm::vec4
    {
        return glm::vec4(particle.offset, static_cast<float>(particle.life)/start);
    };

    std::transform(mParticles.begin(),mParticles.end(),mOffsets.begin(),offsetPack);
}

void ParticleSystem::upload()
{
    glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::vec4)*4096,mOffsets.data());
}

ParticleRenderer::ParticleRenderer() : mTexture(GLTexture())
{
    mParticleSystems.clear();
    mModels.clear();
}

ParticleRenderer::~ParticleRenderer()
{
    mParticleSystems.clear();
    mModels.clear();
}

void ParticleRenderer::addParticleEffect(const unsigned int& ID,const ParticleSystem& particleEffect)
{
    GLModel model;
    model.addVertexAttribute(GL_FLOAT,4);
    model.addVec4(0,0,0,0,0);
    model.createVertexBuffers();
    mModels[ID] = model;
    mParticleSystems[ID] = particleEffect;
}

void ParticleRenderer::update(const unsigned int& delta)
{
    for(auto& system: mParticleSystems)
    {
        system.second.update(delta);
    }

    for(auto iter = mParticleSystems.begin();iter != mParticleSystems.end();)
    {
        if(iter->second.mEffectLife <= 0)
        {
            iter = mParticleSystems.erase(iter);
        }
        else
        {
            iter++;
        }
    }

    auto packLight = [](std::pair<const unsigned int,ParticleSystem>& system) -> Light
    {
        Light l;
        l.position = glm::vec4(system.second.mOrigin + glm::vec3(0,0,-10),25);
        l.colour = glm::vec4(3,0,0,1);
        return l;
    };

    mLights.resize(mParticleSystems.size());
    std::transform(mParticleSystems.begin(),mParticleSystems.end(),mLights.begin(),packLight);
}


void ParticleRenderer::draw()
{
    glEnable(GL_BLEND);
    //glEnable(GL_ALPHA_TEST);
    //glBlendFuncSeparate(GL_SRC_COLOR,GL_DST_COLOR,GL_SRC_ALPHA,GL_DST_ALPHA);
    //glBlendEquationSeparate(GL_FUNC_ADD,GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC1_ALPHA);
    mTexture.bindTexture(0);
    for(auto& system: mParticleSystems)
    {
        system.second.upload();
        mModels[system.first].draw(GL_POINTS,system.second.mParticles.size());
    }
    glDisable(GL_BLEND);
    //glDisable(GL_ALPHA_TEST);
}
