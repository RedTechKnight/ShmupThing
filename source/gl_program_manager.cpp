#include "../headers/gl_program_manager.h"

GLProgramManager::GLProgramManager() : last_uniform_bind_point(0) {
  programs.clear();
  shaders.clear();
  uniform_blocks.clear();
  uniforms.clear();
}

GLProgramManager::~GLProgramManager() {
  for (auto &program : programs) {
    delete_program(program.second);
  }
  uniform_blocks.clear();
  uniforms.clear();
}

void GLProgramManager::create_program(const unsigned int &program_id) {
  programs[program_id] = glCreateProgram();
}

bool GLProgramManager::load_shader(const std::string &filename,
                                   const GLenum &shader_type,
                                   const unsigned int &progran_id) {
  if (programs.size() < 1)
    return false;

  if (glIsProgram(programs[progran_id]) == GL_TRUE) {
    std::fstream file;
    file.open(filename.c_str(),
              std::ios::in | std::ios::binary | std::ios::ate);
    if (file.good()) {
      uint32_t filesize = file.tellg();
      char *data = new char[filesize + 1];
      file.seekg(0, std::ios::beg);
      file.read(data, filesize);
      data[filesize] = '\0';
      file.close();

      GLuint shader = glCreateShader(shader_type);
      const GLchar *source[] = {data};
      glShaderSource(shader, 1, source, 0);
      glCompileShader(shader);

      GLint compiled = GL_FALSE;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
      if (compiled == GL_FALSE) {
        std::cout << "GL Error: " << glGetError() << std::endl;
        print_shader_log(shader);
        return false;
      }
      GLShader new_shader;
      new_shader.program = programs[progran_id];
      new_shader.shader = shader;
      shaders.push_back(new_shader);
      glAttachShader(programs[progran_id], shader);

#ifdef DEBUG
      print_shader_log(shader);
#endif

      return true;
    }
    std::cout << "The file " << filename << " could not be found or opened.\n";
    return false;
  }
  std::cout << "A valid program object with ID of " << progran_id
            << " was not found.\n";
  return false;
}

void GLProgramManager::link_program(const unsigned int &program_id) {
  if (glIsProgram(programs[program_id])) {
    glLinkProgram(programs[program_id]);
    int status = 0;
    glGetProgramiv(programs[program_id], GL_LINK_STATUS, &status);
    if (!status) {
      std::cout << "The program object " << programs[program_id]
                << " could not be linked successfully\n";
      std::cout << "GL Error: " << glGetError() << std::endl;
      print_program_log(programs[program_id]);
    }
  } else {
    std::cout << "No program with ID " << program_id << " exists.\n";
  }
}

void GLProgramManager::use_program(const unsigned int &program_id) {
  if (glIsProgram(programs[program_id])) {
    glUseProgram(programs[program_id]);
  } else if (program_id == 0) {
    glUseProgram(0);
  } else {
    std::cout << "No program with ID " << program_id << " exists." << std::endl;
  }
}

void GLProgramManager::get_uniform(const std::string &name_in_shader,
                                   const unsigned int &program_id,
                                   const unsigned int &uniform_id) {
  GLint loc =
      glGetUniformLocation(programs[program_id], name_in_shader.c_str());
  if (loc < 0) {
    std::cout << "Failed to get the location of the uniform variable '"
              << name_in_shader << "' GL Error: " << glGetError() << std::endl;
  }
  uniforms[uniform_id] = loc;
}

void GLProgramManager::create_uniform_block(
    const unsigned int &program_id, const std::string &name_in_shader,
    const unsigned int &uniform_block_id) {
  GLuint location =
      glGetUniformBlockIndex(programs[program_id], name_in_shader.c_str());

  glUniformBlockBinding(programs[program_id], location,
                        last_uniform_bind_point);
  GLuint ubo;
  glGenBuffers(1, &ubo);
  glBindBufferBase(GL_UNIFORM_BUFFER, last_uniform_bind_point, ubo);
  UniformBlock block;
  block.binding_point = last_uniform_bind_point;
  block.uniform_buffer = ubo;
  uniform_blocks[uniform_block_id] = block;
  last_uniform_bind_point++;
}

void GLProgramManager::bind_uniform_block(
    const unsigned int &uniform_block_id) {
  glBindBufferBase(GL_UNIFORM_BUFFER,
                   uniform_blocks[uniform_block_id].binding_point,
                   uniform_blocks[uniform_block_id].uniform_buffer);
}

void GLProgramManager::print_shader_log(GLuint shader) {
  if (glIsShader(shader)) {
    int info_log_length = 0;
    int max_length = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

    char *info_log = new char[max_length];

    glGetShaderInfoLog(shader, max_length, &info_log_length, info_log);
    if (info_log_length > 0) {
      std::cout << "Info log for shader " << shader << std::endl
                << info_log << std::endl;
    }
    delete[] info_log;
  } else {
    std::cout << "Shader " << shader << " is not a valid shader object.\n";
  }
}

void GLProgramManager::print_program_log(GLuint program) {
  if (glIsProgram(program)) {
    int info_log_length = 0;
    int max_length = 0;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

    char *infoLog = new char[max_length];

    glGetProgramInfoLog(program, max_length, &info_log_length, infoLog);
    if (info_log_length > 0) {
      std::cout << "Info log for program " << program << "\n"
                << infoLog << std::endl;
    }
    delete[] infoLog;
  } else {
    std::cout << "Program " << program << " is not a valid program object.\n";
  }
}

void GLProgramManager::delete_program(const unsigned int &program_id) {
  std::vector<std::list<GLShader>::iterator> to_delete;

  for (auto iter = shaders.begin(); iter != shaders.end(); iter++) {
    if (iter->program == programs[program_id]) {
      to_delete.push_back(iter);
      glDetachShader(iter->program, iter->shader);
      glDeleteShader(iter->shader);
    }
  }

  glDeleteProgram(programs[program_id]);

  for (auto &iter : to_delete) {
    shaders.erase(iter);
  }
  to_delete.clear();
}
