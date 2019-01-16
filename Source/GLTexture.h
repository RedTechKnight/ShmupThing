#ifndef GL_TEXTURE_H_INCLUDED
#define GL_TEXTURE_H_INCLUDED
#include "sdl_gl_window.h"
#include <FreeImage.h>
class GLTexture
{
private:

    uint32_t mBitDepth;
    GLuint mTextureID;
    GLenum mInputFormat;
    GLenum mOutputFormat;
    GLenum mTextureType;
    GLenum mTextureDataType;
    unsigned char* mTextureData;
    FIBITMAP* mImage;
public:
    uint32_t mWidth;
    uint32_t mHeight;

    GLTexture();
    ~GLTexture();
    void loadFromFile(std::string filename,const int& index);
    void loadFromSDLSurface(SDL_Surface *surface,const int& index);
    void reloadTextureFromSDLSurface(SDL_Surface *surface,const int& index);
    void bindTexture(int index);
    GLuint getID();
};

#endif // GL_TEXTURE_H_INCLUDED
