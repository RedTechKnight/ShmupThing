#ifndef SDLGL_H_INCLUDED
#define SDLGL_H_INCLUDED
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

class GLWindow {
private:
  SDL_GLContext gl_context;
  std::string window_name;
  uint32_t window_height;
  uint32_t window_width;
  const GLubyte *max_gl_version;
  const GLubyte *gl_renderer;
  uint32_t gl_major_version, gl_minor_version;
  bool is_fullscreen;
  SDL_Window *window;

public:
  std::map<unsigned int, std::vector<SDL_DisplayMode>> display_modes;

  GLWindow();
  ~GLWindow();

  void initSDL();
  bool create_window(std::string window_name, uint32_t width, uint32_t height,
                     uint32_t major_ver, uint32_t minor_ver);

  void setDisplayMode(SDL_DisplayMode &mode);
  void setFullscreen(const bool &fullscreen);
  bool isFullscreen();

  void clearViewport(const float &r, const float &g, const float &b,
                     const float &a);
  void swapWindow();
};

#endif
