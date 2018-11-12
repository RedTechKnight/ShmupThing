#ifndef GL_PROGRAMMANAGER_H_INCLUDED
#define GL_PROGRAMMANAGER_H_INCLUDED
#include "Dragon_SDLGL.h"
#include <list>
#include <map>
#include <vector>
#include <string>
#include <fstream>

struct UniformBlock
{
    GLuint uniformBuffer;
    GLuint bindingPoint;
};

struct GLShader
{
    GLuint program;
    GLuint shader;
};
class GLProgramManager
{
private:
    unsigned int mLastUniformBindPoint;
    std::list<GLShader> mShaders;
public:
    std::map<unsigned int,GLint> mUniforms;
    std::map<unsigned int,GLuint> mPrograms;
    std::map<unsigned int,UniformBlock> mUniformBlocks;

    GLProgramManager();
    ~GLProgramManager();

    void createProgram(const unsigned int& programID);
    bool loadShader(const std::string& filename,const GLenum& shaderType,const unsigned int& programID);
    void linkProgram(const unsigned int& programName);
    void useProgram(const unsigned int& programName);

    void getUniform(const std::string& nameInShader,const unsigned int& programID,const unsigned int& uniformID);

    void createUniformBlock(const unsigned int& program,const std::string& nameInShader,const unsigned int& uniformBlockID);
    void bindUniformBlock(const unsigned int& uniformBlockID);

    void printShaderLog(GLuint shader);
    void printProgramLog(GLuint program);

    void deleteProgram(const unsigned int& uniform);
};


#endif // GL_PROGRAMMANAGER_H_INCLUDED
