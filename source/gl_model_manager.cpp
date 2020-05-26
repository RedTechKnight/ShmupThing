#include "../headers/gl_model_manager.h"

GLModelManager::GLModelManager() { models.clear(); }

GLModelManager::~GLModelManager() {
  for (auto &model : models) {
    model.second.clean();
  }
}

void GLModelManager::load_from_file(const std::string &filename,
                                    const unsigned int &flags,
                                    const unsigned int &ID) {
  GLModel model;
  model.assimp_load(filename, flags);
  model.create_vertex_buffers();
  model.create_index_buffers();
  models[ID] = model;
}

void GLModelManager::batch_load(const std::string &filename) {
  std::ifstream infile(filename, std::ios::in);

  while (infile.good()) {
    std::string value;
    std::string model_file;
    unsigned int ID;

    infile >> value;
    if (value == "model") {
      infile >> model_file >> ID;
      load_from_file(model_file, aiProcessPreset_TargetRealtime_MaxQuality, ID);
    }
  }
  infile.close();
}

void GLModelManager::addModel(const GLModel &model, const unsigned int &ID) {
  models[ID] = model;
}

void GLModelManager::draw(const unsigned int &ID, const GLenum &draw_type,
                          const unsigned int &instance_count) {
  models[ID].draw(draw_type, instance_count);
}
