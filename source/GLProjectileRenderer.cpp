#include "../headers/GLProjectileRenderer.h"

GLProjectileRenderer::GLProjectileRenderer() : mTexture(GLTexture()),mModel(GLModel())
{
}

GLProjectileRenderer::~GLProjectileRenderer()
{
    mModel.clean();
}

void GLProjectileRenderer::init()
{
    mModel.addVertexAttribute(GL_FLOAT,4);
    mModel.addVec4(0,0,0,0,0);
    mModel.createVertexBuffers();
}

void GLProjectileRenderer::draw(const unsigned int& particleCount)
{
    if(particleCount)
    {
         glEnable(GL_BLEND);
         glBlendFuncSeparate(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
         glBlendEquationSeparate(GL_FUNC_ADD,GL_FUNC_ADD);
         mTexture.bindTexture(0);
         mModel.draw(GL_POINTS,particleCount);
         glDisable(GL_BLEND);
    }
}
