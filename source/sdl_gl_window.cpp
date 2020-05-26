#include "../headers/sdl_gl_window.h"

GLWindow::GLWindow()
    : window_name(" "), window_height(0), window_width(0), gl_minor_version(0),
      gl_major_version(0), is_fullscreen(false) {
  window = nullptr;
}

GLWindow::~GLWindow() {
  if (window) {
    SDL_DestroyWindow(window);
  }
  window = nullptr;

  SDL_GL_DeleteContext(gl_context);
  SDL_Quit();
}

bool GLWindow::create_window(std::string name, uint32_t width, uint32_t height,
                            uint32_t major_ver, uint32_t minor_ver) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "Failed to initialise SDL!" << std::endl;
    return false;
  }
  window_width = width;
  window_height = height;
  window_name = name;
  gl_major_version = major_ver;
  gl_minor_version = minor_ver;

  unsigned int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
  if (is_fullscreen) {
    flags |= SDL_WINDOW_FULLSCREEN;
  }
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_version);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_version);

#ifdef DEBUG
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
  glEnable(GL_DEBUG_OUTPUT);
#endif

  window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, window_width, window_height,
                            flags);
  if (!window) {
    std::cout << "Failed to create the window! Error: " << SDL_GetError()
              << std::endl;
    return false;
  }

  gl_context = SDL_GL_CreateContext(window);
  if (gl_context == nullptr) {
    std::cout << "Failed to create the SDL OpenGL context! Error: "
              << SDL_GetError() << std::endl;
    return false;
  }
  glewExperimental = GL_TRUE;
  GLenum glew_error = glewInit();
  max_gl_version = glGetString(GL_VERSION);
  gl_renderer = glGetString(GL_RENDERER);
  int out_major_ver, out_minor_ver = 0;
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &out_major_ver);
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &out_minor_ver);
  std::cout << "OpenGL driver version is " << max_gl_version
            << "\n Rendering device is " << gl_renderer
            << "\n. Current GL Version is " << out_major_ver << "."
            << out_minor_ver << "\n. Setting to version " << gl_major_version
            << "." << gl_minor_version << std::endl;

  if (glew_error != GLEW_OK) {
    std::cout << "Failed to initialise GLEW! Error: "
              << glewGetErrorString(glew_error) << std::endl;
    return false;
  }

  if (SDL_GL_SetSwapInterval(0) < 0) {
    std::cout << "Failed to set VSync! Error: " << SDL_GetError() << std::endl;
    return false;
  }

  int num_displays = SDL_GetNumVideoDisplays();
  for (unsigned int i = 0; i < num_displays; i++) {
    int display_mode_count = SDL_GetNumDisplayModes(i);
    if (display_mode_count > 0) {
      for (unsigned int j = 0; j < display_mode_count; j++) {
        SDL_DisplayMode mode;
        SDL_GetDisplayMode(i, j, &mode);
        if (mode.format == SDL_PIXELFORMAT_RGB888)
          display_modes[i].push_back(mode);
      }
    }
  }
  return true;
}

void GLWindow::setDisplayMode(SDL_DisplayMode &mode) {
  if (is_fullscreen) {
    SDL_SetWindowDisplayMode(window, &mode);
  } else {
    SDL_SetWindowSize(window, mode.w, mode.h);
  }
}

void GLWindow::setFullscreen(const bool &fs) {
  if (!is_fullscreen) {
    if (fs) {
      SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
      is_fullscreen = fs;
    }
  }
  if (is_fullscreen) {
    if (!fs) {
      SDL_SetWindowFullscreen(window, 0);
      is_fullscreen = fs;
    }
  }
}

bool GLWindow::isFullscreen() { return is_fullscreen; }

void GLWindow::clearViewport(const float &r, const float &g, const float &b,
                             const float &a) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(r, g, b, a);
}

void GLWindow::swapWindow() { SDL_GL_SwapWindow(window); }
