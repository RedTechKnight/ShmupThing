#include "dragon_sdlgl.h"

DragonGL::DragonGL() : mWindowName(" "),mWindowHeight(0),mWindowWidth(0),mGLMinorVer(0),mGLMajorVer(0),mFullscreen(false)
{
    mWindow = nullptr;
}

DragonGL::~DragonGL()
{
    if(mWindow)
    {
        SDL_DestroyWindow(mWindow);
    }
    mWindow = 0;
    SDL_GL_DeleteContext(mGLContext);
    mWindowName = "";
    mWindowHeight = mWindowWidth = 0;
    mGLMinorVer = 1;
    mGLMajorVer = 2;
    TTF_Quit();
    SDL_Quit();
}

bool DragonGL::initSDL(std::string name,uint32_t width,uint32_t height,uint32_t majorVersion,uint32_t minorVersion)
{
    if(SDL_Init(SDL_INIT_EVERYTHING)<0)
    {
        std::cout << "Failed to initialise SDL!"<<std::endl;
        return false;
    }
    if(TTF_Init() < 0)
    {
        std::cout<<"Failed to initialise the font subsystem!"<<std::endl;
        return false;
    }
    SDL_ShowCursor(SDL_DISABLE);
    mWindowWidth = width;
    mWindowHeight = height;
    mWindowName = name;
    mGLMajorVer = majorVersion;
    mGLMinorVer = minorVersion;
    unsigned int flags = SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL;
    if(mFullscreen)
    {
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    mWindow = SDL_CreateWindow(name.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,mWindowWidth,mWindowHeight,flags);
    if(!mWindow)
    {
        std::cout << "Failed to create the window! Error: "<< SDL_GetError() <<std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);

    #ifdef DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,SDL_GL_CONTEXT_DEBUG_FLAG);
        glEnable(GL_DEBUG_OUTPUT);
    #endif

    mGLContext = SDL_GL_CreateContext(mWindow);
    if(mGLContext == nullptr)
    {
        std::cout<< "Failed to create the SDL OpenGL context! Error: "<<SDL_GetError()<<std::endl;
        return false;
    }
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    mMaxGLVersion = glGetString(GL_VERSION);
    mGLRenderer = glGetString(GL_RENDERER);
    int minorVer,majorVer;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,&majorVer);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,&minorVer);
    std::cout<< "OpenGL driver version is "<< mMaxGLVersion<<"\n Rendering device is "<<mGLRenderer<<"\n. Current GL Version is "<<majorVer<<"."<<minorVer<<"\n. Setting to version "<<mGLMajorVer<<"."<<mGLMinorVer<<std::endl;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,mGLMajorVer);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,mGLMinorVer);

    if(glewError != GLEW_OK)
    {
        std::cout<<"Failed to initialise GLEW! Error: "<<glewGetErrorString(glewError)<<std::endl;
        return false;
    }

    if(SDL_GL_SetSwapInterval(0)<0)
    {
        std::cout<<"Failed to set VSync! Error: "<<SDL_GetError()<<std::endl;
        return false;
    }

    int numDisplays = SDL_GetNumVideoDisplays();
    for(unsigned int i = 0;i < numDisplays;i++)
    {
        int displayModes = SDL_GetNumDisplayModes(i);
        if(displayModes > 0)
        {
            for(unsigned int j = 0;j < displayModes;j++)
            {
                SDL_DisplayMode mode;
                SDL_GetDisplayMode(i,j,&mode);
                if(mode.format == SDL_PIXELFORMAT_RGB888)
                    mDisplayModes[i].push_back(mode);
            }
        }
    }
}

void DragonGL::setFullscreen(const bool& fs)
{
    if(!mFullscreen)
    {
        if(fs)
        {
            SDL_SetWindowFullscreen(mWindow,SDL_WINDOW_FULLSCREEN);
            mFullscreen = fs;
        }
    }
    if(mFullscreen)
    {
        if(!fs)
        {
            SDL_SetWindowFullscreen(mWindow,0);
            mFullscreen = fs;
        }
    }
}

bool DragonGL::isFullscreen()
{
    return mFullscreen;
}

void DragonGL::clearViewport(const float& r,const float& g,const float& b,const float& a)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(r,g,b,a);
}

void DragonGL::swapWindow()
{
    SDL_GL_SwapWindow(mWindow);
}
