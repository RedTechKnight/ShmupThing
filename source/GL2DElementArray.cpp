#include "../headers/GL2DElementArray.h"

GL2DElementArray::GL2DElementArray()
    : texture_array(0), vertex_buffer(0), vertex_array_object(0),
      element_count(0), is_image_rects_changed(false),
      is_texture_areas_changed(false), screen_width(0), screen_height(0) {
  for (unsigned int i = 0; i < 40; i++) {
    image_rects[i * 2] = 0;
    image_rects[i * 2 + 1] = 0;
    texture_areas[i * 2] = 0;
    texture_areas[i * 2 + 1] = 0;
    tex_coord_scales[i] = 0;
  }
  for (unsigned int i = 0; i < 20; i++) {
    depths[i] = 0;
  }
}

GL2DElementArray::~GL2DElementArray() {
  for (unsigned int i = 0; i < 80; i++) {
    image_rects[i] = 0;
    texture_areas[i] = 0;
  }
}

void GL2DElementArray::init() {
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  float data = 0;
  glBufferData(GL_ARRAY_BUFFER, sizeof(float), &data, GL_STATIC_DRAW);
  glGenVertexArrays(1, &vertex_array_object);
  glBindVertexArray(vertex_array_object);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 1, GL_FLOAT, false, 0, nullptr);
  glGenTextures(1, &texture_array);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, 1024, 1024, 20, 0, GL_BGRA,
               GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void GL2DElementArray::set_element_rect(const unsigned int &index,
                                        const float &x, const float &y,
                                        const float &width,
                                        const float &height) {
  is_image_rects_changed = true;
  image_rects[index * 4] = x;
  image_rects[index * 4 + 1] = y;
  image_rects[index * 4 + 2] = width;
  image_rects[index * 4 + 3] = height;
}

void GL2DElementArray::set_texture_area(const unsigned int &index,
                                        const float &x, const float &y,
                                        const float &width,
                                        const float &height) {
  is_texture_areas_changed = true;
  texture_areas[index * 4] = x * tex_coord_scales[index * 2];
  texture_areas[index * 4 + 1] = y * tex_coord_scales[index * 2 + 1];
  texture_areas[index * 4 + 2] = width * tex_coord_scales[index * 2];
  texture_areas[index * 4 + 3] = height * tex_coord_scales[index * 2 + 1];
}

void GL2DElementArray::set_element_depth(const unsigned int &index,
                                         const float &depth) {
  is_element_depth_changed = true;
  depths[index] = depth;
}

void GL2DElementArray::add_element(const std::string &filename) {
  FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str());
  FIBITMAP *image = FreeImage_Load(format, filename.c_str());
  unsigned int image_width = FreeImage_GetWidth(image);
  unsigned int image_height = FreeImage_GetHeight(image);
  if (image_width > 1024 || image_height > 1024) {
    image = FreeImage_Rescale(image, 1024, 1024);
    image_width = 1024;
    image_height = 1024;
  }
  image = FreeImage_ConvertTo32Bits(image);
  BYTE *data = FreeImage_GetBits(image);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, element_count, image_width,
                  image_height, 1, GL_BGRA, GL_UNSIGNED_BYTE, data);
  float w = image_width;
  float h = image_height;
  tex_coord_scales[element_count * 2] = w / 1024.0f;
  tex_coord_scales[element_count * 2 + 1] = h / 1024.0f;
  element_count++;
}

void GL2DElementArray::draw() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
  if (is_image_rects_changed) {
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 80,
                    image_rects.data());
  }
  if (is_texture_areas_changed) {
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 80, sizeof(float) * 80,
                    texture_areas.data());
  }
  if (is_element_depth_changed) {
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 160, sizeof(float) * 20,
                    depths.data());
  }
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindVertexArray(vertex_array_object);
  glDrawArraysInstanced(GL_POINTS, 0, 1, element_count);
  glDisable(GL_BLEND);
}

void GL2DElementArray::reset() {
  for (unsigned int i = 0; i < 40; i++) {
    image_rects[i * 2] = 0;
    image_rects[i * 2 + 1] = 0;
    texture_areas[i * 2] = 0;
    texture_areas[i * 2 + 1] = 0;
    tex_coord_scales[i] = 0;
  }
  for (unsigned int i = 0; i < 20; i++) {
    depths[i] = 0;
  }
  element_count = 0;
}
