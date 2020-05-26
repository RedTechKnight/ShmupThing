#include "../headers/GLModelManager.h"

GLModelManager::GLModelManager()
{
    mModels.clear();
}

GLModelManager::~GLModelManager()
{
    for(auto& model : mModels)
    {
        model.second.clean();
    }
}


void GLModelManager::loadFromFile(const std::string& filename,const unsigned int& flags,const unsigned int& ID)
{
    GLModel model;
    model.assImpLoad(filename,flags);
    model.createVertexBuffers();
    model.createIndexBuffers();
    mModels[ID] = model;
}

void GLModelManager::batchLoad(const std::string& filename)
{
    std::ifstream infile(filename,std::ios::in);

    while(infile.good())
    {
        std::string value;
        std::string modelfile;
        unsigned int ID;

         infile >> value;
         if(value == "model")
         {
             infile >> modelfile >> ID;
             loadFromFile(modelfile,aiProcessPreset_TargetRealtime_MaxQuality,ID);
         }
    }
    infile.close();
}

void GLModelManager::addModel(const GLModel& model,const unsigned int& ID)
{
    mModels[ID] = model;
}

void GLModelManager::draw(const unsigned int& ID,const GLenum& drawType,const unsigned int& instanceCount)
{
    mModels[ID].draw(drawType,instanceCount);
}
