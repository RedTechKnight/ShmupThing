#include "../headers/GLProjectileRenderer.h"

GLProjectileRenderer::GLProjectileRenderer()
    : texture(GLTexture()), model(GLModel()) {}

GLProjectileRenderer::~GLProjectileRenderer() { model.clean(); }

void GLProjectileRenderer::init() {
  model.add_vertex_attribute(GL_FLOAT, 4);
  model.add_vec_4(0, 0, 0, 0, 0);
  model.create_vertex_buffers();
}

void GLProjectileRenderer::draw(const unsigned int &particleCount) {
  if (particleCount) {
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA,
                        GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    texture.bind_texture(0);
    model.draw(GL_POINTS, particleCount);
    glDisable(GL_BLEND);
  }
}
