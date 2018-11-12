#ifndef GLMODELMANAGER_H_INCLUDED
#define GLMODELMANAGER_H_INCLUDED
#include "GL_Model.h"
#include <map>
#include <fstream>
#include <istream>
#include <iostream>

class GLModelManager
{
private:
    std::map<unsigned int,GLModel> mModels;
public:
    GLModelManager();
    ~GLModelManager();

    void batchLoad(const std::string& filename);
    void loadFromFile(const std::string& filename,const unsigned int& assimpProcessFlags,const unsigned int& ID);
    void addModel(const GLModel& model,const unsigned int& ID);

    void draw(const unsigned int& ID,const GLenum& drawType,const unsigned int& instanceCount);
};


#endif // GLMODELMANAGER_H_INCLUDED
