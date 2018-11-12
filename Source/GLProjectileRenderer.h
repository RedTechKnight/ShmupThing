#ifndef GLPROJECTILERENDERER_H_INCLUDED
#define GLPROJECTILERENDERER_H_INCLUDED
#include "GL_Texture.h"
#include "GL_Model.h"
class GLProjectileRenderer
{
private:
    GLModel mModel;
public:
    GLTexture mTexture;

    GLProjectileRenderer();
    ~GLProjectileRenderer();

    void init();
    void draw(const unsigned int& particleCount);
};

#endif // GLPROJECTILERENDERER_H_INCLUDED
