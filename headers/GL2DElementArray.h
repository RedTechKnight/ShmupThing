#ifndef GL2DELEMENTARRAY_H_INCLUDED
#define GL2DELEMENTARRAY_H_INCLUDED
#include "sdl_gl_window.h"
#include <FreeImage.h>
#include <array>
#include <memory>
class GL2DElementArray {
private:
  std::array<float, 80> image_rects;
  std::array<float, 80> texture_areas;
  std::array<float, 40> tex_coord_scales;
  std::array<float, 20> depths;
  GLuint texture_array;
  GLuint vertex_buffer;
  GLuint vertex_array_object;
  unsigned int element_count;
  bool is_image_rects_changed;
  bool is_texture_areas_changed;
  bool is_element_depth_changed;

public:
  float screen_width;
  float screen_height;
  GL2DElementArray();
  ~GL2DElementArray();

  void init();
  void add_element(const std::string &imageFilename);
  void set_element_rect(const unsigned int &index, const float &x,
                        const float &y, const float &width,
                        const float &height);
  void set_texture_area(const unsigned int &index, const float &x,
                        const float &y, const float &width,
                        const float &height);
  void set_element_depth(const unsigned int &index, const float &depth);
  void draw();
  void reset();
};

#endif // GL2DELEMENTARRAY_H_INCLUDED
