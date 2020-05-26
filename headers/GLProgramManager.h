#ifndef GL_PROGRAMMANAGER_H_INCLUDED
#define GL_PROGRAMMANAGER_H_INCLUDED
#include "sdl_gl_window.h"
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>

struct UniformBlock {
  GLuint uniform_buffer;
  GLuint binding_point;
};

struct GLShader {
  GLuint program;
  GLuint shader;
};
class GLProgramManager {
private:
  unsigned int last_uniform_bind_point;
  std::list<GLShader> shaders;

public:
  std::map<unsigned int, GLint> uniforms;
  std::map<unsigned int, GLuint> programs;
  std::map<unsigned int, UniformBlock> uniform_blocks;

  GLProgramManager();
  ~GLProgramManager();

  void create_program(const unsigned int &programID);
  bool load_shader(const std::string &filename, const GLenum &shaderType,
                   const unsigned int &programID);
  void link_program(const unsigned int &programName);
  void use_program(const unsigned int &programName);

  void get_uniform(const std::string &nameInShader,
                   const unsigned int &programID,
                   const unsigned int &uniformID);

  void create_uniform_block(const unsigned int &program,
                            const std::string &nameInShader,
                            const unsigned int &uniformBlockID);
  void bind_uniform_block(const unsigned int &uniformBlockID);

  void print_shader_log(GLuint shader);
  void print_program_log(GLuint program);

  void delete_program(const unsigned int &uniform);
};

#endif // GL_PROGRAMMANAGER_H_INCLUDED
