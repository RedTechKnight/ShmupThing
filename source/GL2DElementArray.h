#ifndef GL2DELEMENTARRAY_H_INCLUDED
#define GL2DELEMENTARRAY_H_INCLUDED
#include <array>
#include "sdl_gl_window.h"
#include <FreeImage.h>
#include <memory>
class GL2DElementArray
{
private:
    std::array<float,80> mImageRects;
    std::array<float,80> mTextureAreas;
    std::array<float,40> mTexCoordScales;
    std::array<float,20> mDepths;
    GLuint mTextureArray;
    GLuint mVertexBuffer;
    GLuint mVertexArrayObject;
    unsigned int mElementCount;
    bool mImageRectsChanged;
    bool mTextureAreasChanged;
    bool mElementDepthChanged;
public:
    float mScreenWidth;
    float mScreenHeight;
    GL2DElementArray();
    ~GL2DElementArray();

    void init();
    void addElement(const std::string& imageFilename);
    void setElementRect(const unsigned int& index,const float& x,const float& y,const float& width,const float& height);
    void setTextureArea(const unsigned int& index,const float& x,const float& y,const float& width,const float& height);
    void setElementDepth(const unsigned int& index,const float& depth);
    void draw();
    void reset();
};

#endif // GL2DELEMENTARRAY_H_INCLUDED
