#ifndef GLPROJECTILERENDERER_H_INCLUDED
#define GLPROJECTILERENDERER_H_INCLUDED
#include "gl_model.h"
#include "gl_texture.h"
class GLProjectileRenderer {
private:
  GLModel model;

public:
  GLTexture texture;

  GLProjectileRenderer();
  ~GLProjectileRenderer();

  void init();
  void draw(const unsigned int &particleCount);
};

#endif // GLPROJECTILERENDERER_H_INCLUDED
