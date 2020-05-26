#ifndef GL_TEXTURE_H_INCLUDED
#define GL_TEXTURE_H_INCLUDED
#include "sdl_gl_window.h"
#include <FreeImage.h>
class GLTexture {
private:
  uint32_t bit_depth;
  GLuint texture_id;
  GLenum input_format;
  GLenum output_format;
  GLenum texture_type;
  GLenum texture_data_type;
  unsigned char *texture_data;
  FIBITMAP *image;

public:
  uint32_t width;
  uint32_t height;

  GLTexture();
  ~GLTexture();
  void load_from_file(std::string filename, const int &index);
  void load_from_sdl_surface(SDL_Surface *surface, const int &index);
  void reload_texture_from_sdl_surface(SDL_Surface *surface, const int &index);
  void bind_texture(int index);
  GLuint getID();
};

#endif // GL_TEXTURE_H_INCLUDED
