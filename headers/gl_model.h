#ifndef GL_MODEL_H_INCLUDED
#define GL_MODEL_H_INCLUDED
#include "sdl_gl_window.h"
#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stack>
#include <vector>

struct Element {
  GLenum type;
  unsigned int num;
};
class GLModel {
private:
  std::vector<std::vector<float>> vertices;
  std::vector<unsigned int> indices;
  std::vector<Element> vertex_attributes;
  std::vector<GLuint> vertex_buffers;
  GLuint index_buffer;
  GLuint vertex_array_object;
  unsigned int vertex_components;

public:
  GLModel();
  ~GLModel();

  void add_vertex_attribute(GLenum type, unsigned int num);
  void add_element(float elem, unsigned int index);
  void add_vec_2(float x, float y, unsigned int index);
  void add_vec_3(float x, float y, float z, unsigned int index);
  void add_vec_4(float x, float y, float z, float w, unsigned int index);
  void add_index(unsigned int index);

  void assimp_load(const std::string &filename, const int &postProcessingFlags);

  void create_vertex_buffers();
  void create_index_buffers();

  void draw(GLenum primType, const unsigned int &instanceCount);
  void clean();
};

#endif // GL_MODEL_H_INCLUDED
