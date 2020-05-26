#ifndef GL2DELEMENT_H_INCLUDED
#define GL2DELEMENT_H_INCLUDED
#include <array>
#include "GLTexture.h"
class GL2DElement
{
private:

    GLuint mVertexArrayObject;
    GLuint mVertexBuffer;
    std::array<float,4> mImageRect;
    std::array<float,4> mTextureArea;
    bool mPositionUpdated;
    bool mTextureAreaUpdated;
public:
    GLTexture mTexture;
    float mScreenWidth;
    float mScreenHeight;

    GL2DElement();
    ~GL2DElement();

    void init();
    void setTextureArea(const float& x,const float& y,const float& width,const float& height);
    void setElementPosition(const float& x,const float& y,const float& width,const float& height);
    void draw();
};

#endif // GL2DELEMENT_H_INCLUDED
