#ifndef GLTEXT_H_INCLUDED
#define GLTEXT_H_INCLUDED
#include <array>
#include <SDL_ttf.h>
#include "Dragon_SDLGL.h"

class GLTextManager
{
private:
    TTF_Font* mMainFont;
    std::array<float,80> mTextRects;
    std::array<float,40> mTextTextureScales;
    std::array<float,20> mDepths;
    std::array<float,80> mColours;
    GLuint mTextTextureArray;
    GLuint mVertexBuffer;
    GLuint mVertexArrayObject;
    unsigned int mElementCount;
    bool mTextPositionUpdated;
    bool mTextColourUpdated;
    float mScreenWidth;
    float mScreenHeight;
public:

    GLTextManager();
    ~GLTextManager();
    void init();
    void addText(const std::string& text,const float& x,const float& y,const float& width,const float& height,const float& depth,const SDL_Color& fontColor);
    void setTextPosition(const unsigned int& index,const float& x,const float& y,const float& width,const float& height,const float& depth);
    void setText(const unsigned int& index,const std::string& text,const SDL_Color& color);
    void setFont(const std::string& fontFilename,const int& pointSize);
    void setColour(const unsigned int& index,const float& r,const float& g,const float& b);
    void draw();
    void reset();
};

#endif // GLTEXT_H_INCLUDED
