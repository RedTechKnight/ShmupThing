#ifndef GLMULTILINETEXT_H_INCLUDED
#define GLMULTILINETEXT_H_INCLUDED

#include <SDL2/SDL_ttf.h>
#include "GLModel.h"
#include "GLTexture.h"

struct TextParams
{
    std::array<float,64> rect;
    std::array<float,64> area;
};

class GLMultilineText
{
private:
    bool mTextUpdated;
    TextParams mTextParameterList;
    GLTexture mFontTexture;
    SDL_Surface* mFontSurface;
    TTF_Font* mMainFont;
    bool mFirstLoaded;
    GLuint mVertexBuffer;
    GLuint mVertexAttribArray;
    unsigned int mTextRectCount;
public:
    float mScreenWidth;
    float mScreenHeight;

    GLMultilineText();
    ~GLMultilineText();

    void init();
    void setMainFont(const std::string& filename,const unsigned int& pointSize);
    void setText(const float& x,const float& y,const std::string& text,const SDL_Color& colour);
    void drawText();
};


#endif // GLMULTILINETEXT_H_INCLUDED
