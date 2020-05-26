#ifndef GLMODELMANAGER_H_INCLUDED
#define GLMODELMANAGER_H_INCLUDED
#include "gl_model.h"
#include <fstream>
#include <iostream>
#include <istream>
#include <map>

class GLModelManager {
private:
  std::map<unsigned int, GLModel> models;

public:
  GLModelManager();
  ~GLModelManager();

  void batch_load(const std::string &filename);
  void load_from_file(const std::string &filename,
                      const unsigned int &assimpProcessFlags,
                      const unsigned int &ID);
  void addModel(const GLModel &model, const unsigned int &ID);

  void draw(const unsigned int &ID, const GLenum &drawType,
            const unsigned int &instanceCount);
};

#endif // GLMODELMANAGER_H_INCLUDED
