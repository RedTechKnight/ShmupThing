#include "../headers/GL2DElement.h"

GL2DElement::GL2DElement()
    : texture(GLTexture()), screen_width(0), screen_height(0) {}

GL2DElement::~GL2DElement() {}

void GL2DElement::init() {
  glGenVertexArrays(1, &vertex_array_object);
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  float data = 0;
  glBufferData(GL_ARRAY_BUFFER, sizeof(float), &data, GL_STATIC_DRAW);
  glBindVertexArray(vertex_array_object);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 1, GL_FLOAT, false, 0, nullptr);
}

void GL2DElement::set_element_position(const float &x, const float &y,
                                       const float &width,
                                       const float &height) {
  is_position_updated = true;
  image_rect[0] = x;
  image_rect[1] = y;
  image_rect[2] = width;
  image_rect[3] = height;
}

void GL2DElement::set_texture_area(const float &x, const float &y,
                                   const float &width, const float &height) {
  is_texture_area_updated = true;
  texture_area[0] = x;
  texture_area[1] = y;
  texture_area[2] = width;
  texture_area[3] = height;
}

void GL2DElement::draw() {
  if (is_position_updated) {
    is_position_updated = false;
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 4, image_rect.data());
  }
  if (is_texture_area_updated) {
    is_texture_area_updated = false;
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 4, sizeof(float) * 4,
                    texture_area.data());
  }
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  texture.bind_texture(0);
  glBindVertexArray(vertex_array_object);
  glDrawArrays(GL_POINTS, 0, 1);
  glDisable(GL_BLEND);
}
