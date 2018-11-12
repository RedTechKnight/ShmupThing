#ifndef GL_MODEL_H_INCLUDED
#define GL_MODEL_H_INCLUDED
#include "Dragon_SDLGL.h"
#include <algorithm>
#include <vector>
#include <stack>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct element
{
    GLenum type;
    unsigned int num;
};
class GLModel
{
private:
    std::vector<std::vector<float>> mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<element> mVertexAttributes;
    std::vector<GLuint> mVertexBuffers;
    GLuint mIndexBuffer;
    GLuint mVertexArrayObject;
    unsigned int mVertexComponents;
public:
    GLModel();
    ~GLModel();

    void addVertexAttribute(GLenum type,unsigned int num);
    void addElement(float elem,unsigned int index);
    void addVec2(float x,float y,unsigned int index);
    void addVec3(float x,float y,float z,unsigned int index);
    void addVec4(float x,float y,float z,float w,unsigned int index);
    void addIndex(unsigned int index);

    void assImpLoad(const std::string& filename,const int& postProcessingFlags);

    void createVertexBuffers();
    void createIndexBuffers();

    void draw(GLenum primType,const unsigned int& instanceCount);
    void clean();
};

#endif // GL_MODEL_H_INCLUDED
