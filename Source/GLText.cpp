#include "GLText.h"

GLTextManager::GLTextManager() : mTextTextureArray(0),mVertexBuffer(0),
mVertexArrayObject(0),mElementCount(0),mTextPositionUpdated(false),mTextColourUpdated(false),
mScreenWidth(0),mScreenHeight(0)
{
    for(int i = 0;i < 20;i++)
    {
        mDepths[i] = 0;
        mTextTextureScales[i*2] = 0;
        mTextTextureScales[i*2+1] = 0;
        mTextRects[i*4] = 0;
        mTextRects[i*4+1] = 0;
        mTextRects[i*4+2] = 0;
        mTextRects[i*4+3] = 0;
        mColours[i*4] = 1;
        mColours[i*4+1] = 1;
        mColours[i*4+2] = 1;
        mColours[i*4+3] = 1;
    }
}

GLTextManager::~GLTextManager()
{
    glDeleteTextures(1,&mTextTextureArray);
    glDeleteVertexArrays(1,&mVertexArrayObject);
    glDeleteBuffers(1,&mVertexBuffer);
    if(mMainFont)
        TTF_CloseFont(mMainFont);

}

void GLTextManager::init()
{
    glGenBuffers(1,&mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,mVertexBuffer);
    float data = 0;
    glBufferData(GL_ARRAY_BUFFER,sizeof(float),&data,GL_STATIC_DRAW);

    glGenVertexArrays(1,&mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,1,GL_FLOAT,GL_FALSE,0,nullptr);

    glGenTextures(1,&mTextTextureArray);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY,mTextTextureArray);
    glTexImage3D(GL_TEXTURE_2D_ARRAY,0,GL_RGBA8,2048,256,20,0,GL_BGRA,GL_UNSIGNED_BYTE,nullptr);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void GLTextManager::addText(const std::string& text,const float& x,const float& y,const float& width,const float& height,const float& depth,const SDL_Color& color)
{
    mTextPositionUpdated = true;
    SDL_Surface* textSurface = TTF_RenderText_Blended(mMainFont,text.c_str(),color);
    unsigned char* data = nullptr;
    float textWidth,textHeight;
    if(textSurface)
    {
        SDL_LockSurface(textSurface);
        textWidth = textSurface->w;
        if(textWidth > 2048)
        {
            textWidth = 2048;
        }
        textHeight = textSurface->h;

        if(textHeight > 256)
        {
            textHeight = 256;
        }
        data = static_cast<unsigned char*>(textSurface->pixels);
        SDL_UnlockSurface(textSurface);

        mTextTextureScales[mElementCount*2] = textWidth/2048.0f;
        mTextTextureScales[mElementCount*2+1] = textHeight/256.0f;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY,mTextTextureArray);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,mElementCount,textWidth,textHeight,1,GL_BGRA,GL_UNSIGNED_BYTE,data);
        mTextRects[mElementCount*4] = x;
        mTextRects[mElementCount*4+1] = y;
        mTextRects[mElementCount*4+2] = width;
        mTextRects[mElementCount*4+3] = height;
        mDepths[mElementCount] = depth;
        mElementCount++;
        SDL_FreeSurface(textSurface);
    }
    else
    {
        std::cout<<"Failed to add text: \""<<text<<"\" to the manager. Error: "<<TTF_GetError()<<std::endl;
    }
}

void GLTextManager::setTextPosition(const unsigned int& index,const float& x,const float& y,const float& width,const float& height,const float& depth)
{
    mTextPositionUpdated = true;
    mTextRects[index*4] = x;
    mTextRects[index*4+1] = y;
    mTextRects[index*4+2] = width;
    mTextRects[index*4+3] = height;
    mDepths[index] = depth;
}

void GLTextManager::setText(const unsigned int& index,const std::string& text,const SDL_Color& color)
{
    SDL_Surface* textSurface = TTF_RenderText_Blended(mMainFont,text.c_str(),color);
    unsigned char* data = nullptr;
    float textWidth,textHeight;
    if(textSurface)
    {
        SDL_LockSurface(textSurface);
        textWidth = textSurface->w;
        if(textWidth > 2048)
        {
            textWidth = 2048;
        }
        textHeight = textSurface->h;

        if(textHeight > 256)
        {
            textHeight = 256;
        }
        data = static_cast<unsigned char*>(textSurface->pixels);
        SDL_UnlockSurface(textSurface);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY,mTextTextureArray);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,index,textWidth,textHeight,1,GL_BGRA,GL_UNSIGNED_BYTE,data);
        mTextTextureScales[index*2] = textWidth/2048.0f;
        mTextTextureScales[index*2+1] = textHeight/256.0f;
        SDL_FreeSurface(textSurface);
    }
    else
    {
        std::cout<<"Failed to set text: \""<<text<<"\" to the manager. Error: "<<TTF_GetError()<<std::endl;
    }
}

void GLTextManager::setFont(const std::string& fontFilename,const int& pointSize)
{
    mMainFont = TTF_OpenFont(fontFilename.c_str(),pointSize);
}

void GLTextManager::setColour(const unsigned int& index,const float& r,const float& g,const float& b)
{
    mColours[index*4] = r;
    mColours[index*4+1] = g;
    mColours[index*4+2] = b;
    mTextColourUpdated = true;
}

void GLTextManager::draw()
{
    if(mTextPositionUpdated)
    {
        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(float)*80,mTextRects.data());
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(float)*80,sizeof(float)*40,mTextTextureScales.data());
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(float)*120,sizeof(float)*20,mDepths.data());
    }
    if(mTextColourUpdated)
    {
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(float)*140,sizeof(float)*80,mColours.data());
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY,mTextTextureArray);
    glBindVertexArray(mVertexArrayObject);
    glDrawArraysInstanced(GL_POINTS,0,1,mElementCount);
    glDisable(GL_BLEND);
}

void GLTextManager::reset()
{
    for(int i = 0;i < 20;i++)
    {
        mDepths[i] = 0;
        mTextTextureScales[i*2] = 0;
        mTextTextureScales[i*2+1] = 0;
        mTextRects[i*4] = 0;
        mTextRects[i*4+1] = 0;
        mTextRects[i*4+2] = 0;
        mTextRects[i*4+3] = 0;
        mColours[i*4] = 1;
        mColours[i*4+1] = 1;
        mColours[i*4+2] = 1;
        mColours[i*4+3] = 1;
    }
    mElementCount = 0;
    mTextPositionUpdated = false;
    mTextColourUpdated = false;
}
