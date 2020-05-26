#include "../headers/GLTexture.h"

GLTexture::GLTexture()
{
    mWidth = 0;
    mHeight = 0;
    mBitDepth = 0;
    mTextureID = 0;
    mInputFormat = 0;
    mOutputFormat = 0;
    mTextureType = 0;
    mTextureDataType = 0;
    mTextureData = nullptr;
    mImage = 0;
}

GLTexture::~GLTexture()
{
    mWidth = 0;
    mHeight = 0;
    mBitDepth = 0;
    mTextureID = 0;
    mInputFormat = 0;
    mOutputFormat = 0;
    mTextureType = 0;
    mTextureDataType = 0;
    glDeleteTextures(1,&mTextureID);
    if(mImage)
        FreeImage_Unload(mImage);
    mImage = 0;
    mTextureData = nullptr;
}

void GLTexture::loadFromFile(std::string filename,const int& index)
{
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str());
    mImage = FreeImage_Load(format,filename.c_str());
    mWidth = FreeImage_GetWidth(mImage);
    mHeight = FreeImage_GetHeight(mImage);
    mBitDepth = FreeImage_GetBPP(mImage);
    mTextureData = FreeImage_GetBits(mImage);
    mTextureType = GL_TEXTURE_2D;
    mTextureDataType = GL_UNSIGNED_BYTE;
    if(mBitDepth == 8)
    {
        mInputFormat = GL_R8;
    }
    else if(mBitDepth == 16)
    {
        mInputFormat = GL_RG8;
    }
    else if(mBitDepth == 24)
    {
        mInputFormat = GL_RGB8;
    }
    else if(mBitDepth == 32)
    {
        mInputFormat = GL_RGBA8;
    }

    if(mInputFormat == GL_R8)
        mOutputFormat = GL_RED;
    if(mInputFormat == GL_RG8)
        mOutputFormat = GL_RG;
    if(mInputFormat == GL_RGB8)
        mOutputFormat = GL_BGR;
    if(mInputFormat == GL_RGBA8)
        mOutputFormat = GL_BGRA;

    glGenTextures(1,&mTextureID);
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,mTextureID);
    glTexImage2D(GL_TEXTURE_2D,0,mInputFormat,mWidth,mHeight,0,mOutputFormat,GL_UNSIGNED_BYTE,mTextureData);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::loadFromSDLSurface(SDL_Surface* surface,const int& index)
{

    mTextureType = GL_TEXTURE_2D;
    SDL_LockSurface(surface);
    SDL_PixelFormat* pixelFormat = surface->format;
    mWidth = surface->w;
    mHeight = surface->h;
    mBitDepth = static_cast<uint32_t>(pixelFormat->BitsPerPixel);
    mTextureData = static_cast<unsigned char*>(surface->pixels);
    mTextureDataType = GL_UNSIGNED_BYTE;
    SDL_UnlockSurface(surface);
    if(mBitDepth == 8)
    {
        mInputFormat = GL_R8;
    }
    else if(mBitDepth == 16)
    {
        mInputFormat = GL_RG8;
    }
    else if(mBitDepth == 24)
    {
        mInputFormat = GL_RGB8;
    }
    else if(mBitDepth == 32)
    {
        mInputFormat = GL_RGBA8;
    }

    if(mInputFormat == GL_R8)
        mOutputFormat = GL_RED;
    if(mInputFormat == GL_RG8)
        mOutputFormat = GL_RG;
    if(mInputFormat == GL_RGB8)
        mOutputFormat = GL_BGR;
    if(mInputFormat == GL_RGBA8)
        mOutputFormat = GL_BGRA;

    glGenTextures(1,&mTextureID);
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,mTextureID);
    glTexImage2D(GL_TEXTURE_2D,0,mInputFormat,mWidth,mHeight,0,mOutputFormat,GL_UNSIGNED_BYTE,mTextureData);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::reloadTextureFromSDLSurface(SDL_Surface *surface,const int& index)
{
    mTextureType = GL_TEXTURE_2D;
    SDL_LockSurface(surface);
    SDL_PixelFormat* pixelFormat = surface->format;
    mWidth = surface->w;
    mHeight = surface->h;
    mBitDepth = static_cast<uint32_t>(pixelFormat->BitsPerPixel);
    mTextureData = static_cast<unsigned char*>(surface->pixels);
    mTextureDataType = GL_UNSIGNED_BYTE;
    SDL_UnlockSurface(surface);
    if(mBitDepth == 8)
    {
        mInputFormat = GL_R8;
    }
    else if(mBitDepth == 16)
    {
        mInputFormat = GL_RG8;
    }
    else if(mBitDepth == 24)
    {
        mInputFormat = GL_RGB8;
    }
    else if(mBitDepth == 32)
    {
        mInputFormat = GL_RGBA8;
    }

    if(mInputFormat == GL_R8)
        mOutputFormat = GL_RED;
    if(mInputFormat == GL_RG8)
        mOutputFormat = GL_RG;
    if(mInputFormat == GL_RGB8)
        mOutputFormat = GL_BGR;
    if(mInputFormat == GL_RGBA8)
        mOutputFormat = GL_BGRA;

    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,mTextureID);
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,mWidth,mHeight,mOutputFormat,GL_UNSIGNED_BYTE,mTextureData);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::bindTexture(int index)
{
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,mTextureID);
}

GLuint GLTexture::getID()
{
    return mTextureID;
}
