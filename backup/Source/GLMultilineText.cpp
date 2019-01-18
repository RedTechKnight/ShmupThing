#include "GLMultilineText.h"


GLMultilineText::GLMultilineText() : mTextUpdated(false),mFontTexture(GLTexture()),
mFontSurface(nullptr),mMainFont(nullptr),mFirstLoaded(false),mTextRectCount(0),mScreenWidth(0),mScreenHeight(0)
{
    for(unsigned int i = 0;i < 64;i++)
    {
        mTextParameterList.rect[i] = 0;
        mTextParameterList.area[i] = 0;
    }
}

GLMultilineText::~GLMultilineText()
{
    if(mFontSurface)
    {
        SDL_FreeSurface(mFontSurface);
    }
    if(mMainFont)
    {
        TTF_CloseFont(mMainFont);
    }
}

void GLMultilineText::init()
{
    glGenBuffers(1,&mVertexBuffer);
    glGenVertexArrays(1,&mVertexAttribArray);
    glBindVertexArray(mVertexAttribArray);

    glBindBuffer(GL_ARRAY_BUFFER,mVertexBuffer);
    float data[2];
    data[0] = 0;
    data[1] = 0;
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*2,data,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,false,0,nullptr);
}

void GLMultilineText::setMainFont(const std::string& filename,const unsigned int& pointSize)
{
    if(mMainFont)
    {
        TTF_CloseFont(mMainFont);
    }

    mMainFont = TTF_OpenFont(filename.c_str(),pointSize);
    if(!mMainFont)
    {
        std::cout<<"Failed to load the font "<<filename<<" Error: "<<TTF_GetError()<<std::endl;
    }
}

void GLMultilineText::setText(const float& x,const float& y,const std::string& text,const SDL_Color& col)
{
    std::string finalText;
    std::vector<int> delimPoints;
    delimPoints.push_back(0);
    int count = 0;
    for(unsigned int i = 0;i < text.size();i++)
    {
        if(text[i] == '\n')
        {
            delimPoints.push_back(i-count);
            count++;
        }
        else
        {
            finalText.push_back(text[i]);
        }
    }

    std::vector<int> lengths;
    for(unsigned int i = 0;i < delimPoints.size();i++)
    {
        if(i+1 >= delimPoints.size())
        {
            lengths.push_back(finalText.size()-delimPoints[i]);
        }
        else
        {
            lengths.push_back(delimPoints[i+1]-delimPoints[i]);
        }
    }

    mTextRectCount = delimPoints.size();

    if(mFontSurface)
    {
        SDL_FreeSurface(mFontSurface);
    }

    mFontSurface = TTF_RenderText_Blended(mMainFont,finalText.c_str(),col);
    if(!mFontSurface)
    {
        std::cout<<"Failed to render the text to the surface! Error: "<<TTF_GetError()<<std::endl;
        return;
    }
    if(!mFirstLoaded)
    {
        mFontTexture.loadFromSDLSurface(mFontSurface,0);
        mFirstLoaded = true;
    }
    else
    {
        mFontTexture.reloadTextureFromSDLSurface(mFontSurface,0);
    }

    float fullTextLen = finalText.size();
    float fontWidth = mFontTexture.mWidth;
    float fontHeight = mFontTexture.mHeight;
    if(x+fontWidth > mScreenWidth)
    {
        fontWidth = mScreenWidth-x;
    }

    for(unsigned int i = 0;i < delimPoints.size();i++)
    {
        float textSegLen = lengths[i];
        float delim = delimPoints[i];

        mTextParameterList.rect[i*4] = x/mScreenWidth;
        mTextParameterList.rect[i*4+1] = (y/mScreenHeight) + (i*(fontHeight/mScreenHeight));
        mTextParameterList.rect[i*4+2] = (textSegLen/fullTextLen)*(fontWidth/mScreenWidth);
        mTextParameterList.rect[i*4+3] = (fontHeight/mScreenHeight);

        mTextParameterList.area[i*4] = delim/fullTextLen;
        mTextParameterList.area[i*4+1] = 0;
        mTextParameterList.area[i*4+2] = (delim/fullTextLen)+(textSegLen/fullTextLen);
        mTextParameterList.area[i*4+3] = 1;
    }
    mTextUpdated = true;
}


void GLMultilineText::drawText()
{
    if(mTextUpdated)
    {
        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(TextParams),&mTextParameterList);
        mTextUpdated = false;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    mFontTexture.bindTexture(0);
    glBindVertexArray(mVertexAttribArray);
    glDrawArraysInstanced(GL_POINTS,0,1,mTextRectCount);
    glDisable(GL_BLEND);
}
