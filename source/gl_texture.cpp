#include "../headers/gl_texture.h"

GLTexture::GLTexture() {
  width = 0;
  height = 0;
  bit_depth = 0;
  texture_id = 0;
  input_format = 0;
  output_format = 0;
  texture_type = 0;
  texture_data_type = 0;
  texture_data = nullptr;
  image = 0;
}

GLTexture::~GLTexture() {
  width = 0;
  height = 0;
  bit_depth = 0;
  texture_id = 0;
  input_format = 0;
  output_format = 0;
  texture_type = 0;
  texture_data_type = 0;
  glDeleteTextures(1, &texture_id);
  if (image)
    FreeImage_Unload(image);
  image = 0;
  texture_data = nullptr;
}

void GLTexture::load_from_file(std::string filename, const int &index) {
  FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str());
  image = FreeImage_Load(format, filename.c_str());
  width = FreeImage_GetWidth(image);
  height = FreeImage_GetHeight(image);
  bit_depth = FreeImage_GetBPP(image);
  texture_data = FreeImage_GetBits(image);
  texture_type = GL_TEXTURE_2D;
  texture_data_type = GL_UNSIGNED_BYTE;
  if (bit_depth == 8) {
    input_format = GL_R8;
  } else if (bit_depth == 16) {
    input_format = GL_RG8;
  } else if (bit_depth == 24) {
    input_format = GL_RGB8;
  } else if (bit_depth == 32) {
    input_format = GL_RGBA8;
  }

  if (input_format == GL_R8)
    output_format = GL_RED;
  if (input_format == GL_RG8)
    output_format = GL_RG;
  if (input_format == GL_RGB8)
    output_format = GL_BGR;
  if (input_format == GL_RGBA8)
    output_format = GL_BGRA;

  glGenTextures(1, &texture_id);
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, input_format, width, height, 0, output_format,
               GL_UNSIGNED_BYTE, texture_data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::load_from_sdl_surface(SDL_Surface *surface, const int &index) {

  texture_type = GL_TEXTURE_2D;
  SDL_LockSurface(surface);
  SDL_PixelFormat *pixelFormat = surface->format;
  width = surface->w;
  height = surface->h;
  bit_depth = static_cast<uint32_t>(pixelFormat->BitsPerPixel);
  texture_data = static_cast<unsigned char *>(surface->pixels);
  texture_data_type = GL_UNSIGNED_BYTE;
  SDL_UnlockSurface(surface);
  if (bit_depth == 8) {
    input_format = GL_R8;
  } else if (bit_depth == 16) {
    input_format = GL_RG8;
  } else if (bit_depth == 24) {
    input_format = GL_RGB8;
  } else if (bit_depth == 32) {
    input_format = GL_RGBA8;
  }

  if (input_format == GL_R8)
    output_format = GL_RED;
  if (input_format == GL_RG8)
    output_format = GL_RG;
  if (input_format == GL_RGB8)
    output_format = GL_BGR;
  if (input_format == GL_RGBA8)
    output_format = GL_BGRA;

  glGenTextures(1, &texture_id);
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, input_format, width, height, 0, output_format,
               GL_UNSIGNED_BYTE, texture_data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::reload_texture_from_sdl_surface(SDL_Surface *surface,
                                                const int &index) {
  texture_type = GL_TEXTURE_2D;
  SDL_LockSurface(surface);
  SDL_PixelFormat *pixelFormat = surface->format;
  width = surface->w;
  height = surface->h;
  bit_depth = static_cast<uint32_t>(pixelFormat->BitsPerPixel);
  texture_data = static_cast<unsigned char *>(surface->pixels);
  texture_data_type = GL_UNSIGNED_BYTE;
  SDL_UnlockSurface(surface);
  if (bit_depth == 8) {
    input_format = GL_R8;
  } else if (bit_depth == 16) {
    input_format = GL_RG8;
  } else if (bit_depth == 24) {
    input_format = GL_RGB8;
  } else if (bit_depth == 32) {
    input_format = GL_RGBA8;
  }

  if (input_format == GL_R8)
    output_format = GL_RED;
  if (input_format == GL_RG8)
    output_format = GL_RG;
  if (input_format == GL_RGB8)
    output_format = GL_BGR;
  if (input_format == GL_RGBA8)
    output_format = GL_BGRA;

  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, output_format,
                  GL_UNSIGNED_BYTE, texture_data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::bind_texture(int index) {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, texture_id);
}

GLuint GLTexture::getID() { return texture_id; }
