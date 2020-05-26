#include "GL2DElementArray.h"

GL2DElementArray::GL2DElementArray() : mTextureArray(0),mVertexBuffer(0),mVertexArrayObject(0),
mElementCount(0),mImageRectsChanged(false),
mTextureAreasChanged(false),mScreenWidth(0),mScreenHeight(0)
{
    for(unsigned int i = 0;i < 40;i++)
    {
        mImageRects[i*2] = 0;
        mImageRects[i*2+1] = 0;
        mTextureAreas[i*2] = 0;
        mTextureAreas[i*2+1] = 0;
        mTexCoordScales[i] = 0;
    }
    for(unsigned int i = 0;i < 20;i++)
    {
        mDepths[i] = 0;
    }
}

GL2DElementArray::~GL2DElementArray()
{
    for(unsigned int i = 0;i < 80;i++)
    {
        mImageRects[i] = 0;
        mTextureAreas[i] = 0;
    }
}

void GL2DElementArray::init()
{
    glGenBuffers(1,&mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,mVertexBuffer);
    float data = 0;
    glBufferData(GL_ARRAY_BUFFER,sizeof(float),&data,GL_STATIC_DRAW);
    glGenVertexArrays(1,&mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,1,GL_FLOAT,false,0,nullptr);
    glGenTextures(1,&mTextureArray);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY,mTextureArray);
    glTexImage3D(GL_TEXTURE_2D_ARRAY,0,GL_RGBA8,1024,1024,20,0,GL_BGRA,GL_UNSIGNED_BYTE,nullptr);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void GL2DElementArray::setElementRect(const unsigned int& index,const float& x,const float& y,const float& width,const float& height)
{
    mImageRectsChanged = true;
    mImageRects[index*4] = x;
    mImageRects[index*4+1] = y;
    mImageRects[index*4+2] = width;
    mImageRects[index*4+3] = height;
}

void GL2DElementArray::setTextureArea(const unsigned int& index,const float& x,const float& y,const float& width,const float& height)
{
    mTextureAreasChanged = true;
    mTextureAreas[index*4] = x * mTexCoordScales[index*2];
    mTextureAreas[index*4+1] = y * mTexCoordScales[index*2+1];
    mTextureAreas[index*4+2] = width * mTexCoordScales[index*2];
    mTextureAreas[index*4+3] = height * mTexCoordScales[index*2+1];
}

void GL2DElementArray::setElementDepth(const unsigned int& index,const float& depth)
{
    mElementDepthChanged = true;
    mDepths[index] = depth;
}

void GL2DElementArray::addElement(const std::string& filename)
{
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str());
    FIBITMAP* image = FreeImage_Load(format,filename.c_str());
    unsigned int imageWidth = FreeImage_GetWidth(image);
    unsigned int imageHeight = FreeImage_GetHeight(image);
    if(imageWidth > 1024 || imageHeight > 1024)
    {
        image = FreeImage_Rescale(image,1024,1024);
        imageWidth = 1024;
        imageHeight = 1024;
    }
    image = FreeImage_ConvertTo32Bits(image);
    BYTE* data = FreeImage_GetBits(image);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY,mTextureArray);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,mElementCount,imageWidth,imageHeight,1,GL_BGRA,GL_UNSIGNED_BYTE,data);
    float w = imageWidth;
    float h = imageHeight;
    mTexCoordScales[mElementCount*2] = w/1024.0f;
    mTexCoordScales[mElementCount*2+1] = h/1024.0f;
    mElementCount++;
}

void GL2DElementArray::draw()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY,mTextureArray);
    if(mImageRectsChanged)
    {
        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(float)*80,mImageRects.data());
    }
    if(mTextureAreasChanged)
    {
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(float)*80,sizeof(float)*80,mTextureAreas.data());
    }
    if(mElementDepthChanged)
    {
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(float)*160,sizeof(float)*20,mDepths.data());
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(mVertexArrayObject);
    glDrawArraysInstanced(GL_POINTS,0,1,mElementCount);
    glDisable(GL_BLEND);
}

void GL2DElementArray::reset()
{
        for(unsigned int i = 0;i < 40;i++)
    {
        mImageRects[i*2] = 0;
        mImageRects[i*2+1] = 0;
        mTextureAreas[i*2] = 0;
        mTextureAreas[i*2+1] = 0;
        mTexCoordScales[i] = 0;
    }
    for(unsigned int i = 0;i < 20;i++)
    {
        mDepths[i] = 0;
    }
    mElementCount = 0;
}
