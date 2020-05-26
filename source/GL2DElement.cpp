#include "../headers/GL2DElement.h"

GL2DElement::GL2DElement() : mTexture(GLTexture()),mScreenWidth(0),mScreenHeight(0)
{

}

GL2DElement::~GL2DElement()
{

}

void GL2DElement::init()
{
    glGenVertexArrays(1,&mVertexArrayObject);
    glGenBuffers(1,&mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,mVertexBuffer);
    float data = 0;
    glBufferData(GL_ARRAY_BUFFER,sizeof(float),&data,GL_STATIC_DRAW);
    glBindVertexArray(mVertexArrayObject);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,1,GL_FLOAT,false,0,nullptr);
}

void GL2DElement::setElementPosition(const float& x,const float& y,const float& width,const float& height)
{
    mPositionUpdated = true;
    mImageRect[0] = x;
    mImageRect[1] = y;
    mImageRect[2] = width;
    mImageRect[3] = height;
}

void GL2DElement::setTextureArea(const float& x,const float& y,const float& width,const float& height)
{
    mTextureAreaUpdated = true;
    mTextureArea[0] = x;
    mTextureArea[1] = y;
    mTextureArea[2] = width;
    mTextureArea[3] = height;
}

void GL2DElement::draw()
{
    if(mPositionUpdated)
    {
        mPositionUpdated = false;
        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(float)*4,mImageRect.data());
    }
    if(mTextureAreaUpdated)
    {
        mTextureAreaUpdated = false;
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(float)*4,sizeof(float)*4,mTextureArea.data());
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    mTexture.bindTexture(0);
    glBindVertexArray(mVertexArrayObject);
    glDrawArrays(GL_POINTS,0,1);
    glDisable(GL_BLEND);
}
