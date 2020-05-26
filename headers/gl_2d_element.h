#ifndef GL2DELEMENT_H_INCLUDED
#define GL2DELEMENT_H_INCLUDED
#include "gl_texture.h"
#include <array>
class GL2DElement {
private:
  GLuint vertex_array_object;
  GLuint vertex_buffer;
  std::array<float, 4> image_rect;
  std::array<float, 4> texture_area;
  bool is_position_updated;
  bool is_texture_area_updated;

public:
  GLTexture texture;
  float screen_width;
  float screen_height;

  GL2DElement();
  ~GL2DElement();

  void init();
  void set_texture_area(const float &x, const float &y, const float &width,
                        const float &height);
  void set_element_position(const float &x, const float &y, const float &width,
                            const float &height);
  void draw();
};

#endif // GL2DELEMENT_H_INCLUDED
