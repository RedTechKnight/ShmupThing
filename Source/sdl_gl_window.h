#ifndef DRAGON_SDLGL_H_INCLUDED
#define DRAGON_SDLGL_H_INCLUDED
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <memory>
#include <map>
#include <vector>
#include <iostream>

class GLWindow
{
private:

    SDL_GLContext mGLContext;
    std::string mWindowName;
    uint32_t mWindowHeight;
    uint32_t mWindowWidth;
    const GLubyte* mMaxGLVersion;
    const GLubyte* mGLRenderer;
    uint32_t mGLMajorVer,mGLMinorVer;
    bool mFullscreen;
    SDL_Window* mWindow;
public:
    std::map<unsigned int, std::vector<SDL_DisplayMode> > mDisplayModes;

    GLWindow();
    ~GLWindow();

    void initSDL();
    bool createWindow(std::string windowName,uint32_t width,uint32_t height,uint32_t majorVer,uint32_t minorVer);


    void setDisplayMode(SDL_DisplayMode &mode);
    void setFullscreen(const bool& fullscreen);
    bool isFullscreen();

    void clearViewport(const float& r,const float& g,const float& b,const float& a);
    void swapWindow();


};


#endif // DRAGON_SDLGL_H_INCLUDED
