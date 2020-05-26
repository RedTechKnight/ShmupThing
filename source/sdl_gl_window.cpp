#include "../headers/sdl_gl_window.h"

GLWindow::GLWindow() : mWindowName(" "),mWindowHeight(0),mWindowWidth(0),mGLMinorVer(0),mGLMajorVer(0),mFullscreen(false)
{
    mWindow = nullptr;
}

GLWindow::~GLWindow()
{
    if(mWindow)
    {
        SDL_DestroyWindow(mWindow);
    }
    mWindow = nullptr;

    SDL_GL_DeleteContext(mGLContext);
    SDL_Quit();
}

bool GLWindow::createWindow(std::string name,uint32_t width,uint32_t height,uint32_t majorVersion,uint32_t minorVersion)
{
    if(SDL_Init(SDL_INIT_EVERYTHING)<0)
    {
        std::cout << "Failed to initialise SDL!"<<std::endl;
        return false;
    }
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
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,mGLMajorVer);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,mGLMinorVer);

    #ifdef DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,SDL_GL_CONTEXT_DEBUG_FLAG);
        glEnable(GL_DEBUG_OUTPUT);
    #endif

    mWindow = SDL_CreateWindow(name.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,mWindowWidth,mWindowHeight,flags);
    if(!mWindow)
    {
        std::cout << "Failed to create the window! Error: "<< SDL_GetError() <<std::endl;
        return false;
    }

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
    int majorVer,minorVer = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,&majorVer);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,&minorVer);
    std::cout<< "OpenGL driver version is "<< mMaxGLVersion<<"\n Rendering device is "<<mGLRenderer<<"\n. Current GL Version is "<<majorVer<<"."<<minorVer<<"\n. Setting to version "<<mGLMajorVer<<"."<<mGLMinorVer<<std::endl; 

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
    return true;
}

void GLWindow::setDisplayMode(SDL_DisplayMode& mode)
{
    if(mFullscreen)
    {
        SDL_SetWindowDisplayMode(mWindow,&mode);
    }
    else
    {
        SDL_SetWindowSize(mWindow,mode.w,mode.h);
    } 
}

void GLWindow::setFullscreen(const bool& fs)
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

bool GLWindow::isFullscreen()
{
    return mFullscreen;
}

void GLWindow::clearViewport(const float& r,const float& g,const float& b,const float& a)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(r,g,b,a);
}

void GLWindow::swapWindow()
{
    SDL_GL_SwapWindow(mWindow);
}
