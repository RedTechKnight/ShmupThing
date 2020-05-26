#include "../headers/GLProgramManager.h"

GLProgramManager::GLProgramManager() : mLastUniformBindPoint(0)
{
    mPrograms.clear();
    mShaders.clear();
    mUniformBlocks.clear();
    mUniforms.clear();
}

GLProgramManager::~GLProgramManager()
{
    for(auto& program: mPrograms)
    {
        deleteProgram(program.second);
    }
    mUniformBlocks.clear();
    mUniforms.clear();
}

void GLProgramManager::createProgram(const unsigned int& programID)
{
    mPrograms[programID] = glCreateProgram();
}


bool GLProgramManager::loadShader(const std::string& filename,const GLenum& shaderType,const unsigned int& programID)
{
    if(mPrograms.size() < 1)
        return false;

    if(glIsProgram(mPrograms[programID]) == GL_TRUE)
    {
        std::fstream file;
        file.open(filename.c_str(),std::ios::in|std::ios::binary|std::ios::ate);
        if(file.good())
        {
            uint32_t filesize = file.tellg();
            char* data = new char[filesize+1];
            file.seekg(0,std::ios::beg);
            file.read(data,filesize);
            data[filesize] = '\0';
            file.close();

            GLuint shader = glCreateShader(shaderType);
            const GLchar* source[] = {data};
            glShaderSource(shader,1,source,0);
            glCompileShader(shader);

            GLint compiled = GL_FALSE;
            glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);
            if(compiled == GL_FALSE)
            {
                std::cout<<"GL Error: "<<glGetError()<<std::endl;
                printShaderLog(shader);
                return false;
            }
            GLShader newShader;
            newShader.program = mPrograms[programID];
            newShader.shader = shader;
            mShaders.push_back(newShader);
            glAttachShader(mPrograms[programID],shader);

            #ifdef DEBUG
                printShaderLog(shader);
            #endif

            return true;
        }
        std::cout<<"The file "<<filename<<" could not be found or opened.\n";
        return false;
    }
    std::cout<<"A valid program object with ID of "<<programID<<" was not found.\n";
    return false;
}

void GLProgramManager::linkProgram(const unsigned int& programID)
{
    if(glIsProgram(mPrograms[programID]))
    {
        glLinkProgram(mPrograms[programID]);
        int status = 0;
        glGetProgramiv(mPrograms[programID],GL_LINK_STATUS,&status);
        if(!status)
        {
            std::cout<<"The program object "<<mPrograms[programID]<<" could not be linked successfully\n";
            std::cout<<"GL Error: "<<glGetError()<<std::endl;
            printProgramLog(mPrograms[programID]);
        }
    }
    else
    {
        std::cout<<"No program with ID "<<programID<<" exists.\n";
    }
}

void GLProgramManager::useProgram(const unsigned int& programID)
{
    if(glIsProgram(mPrograms[programID]))
    {
        glUseProgram(mPrograms[programID]);
    }
    else if(programID == 0)
    {
        glUseProgram(0);
    }
    else
    {
        std::cout<<"No program with ID "<<programID<<" exists."<<std::endl;
    }
}

void GLProgramManager::getUniform(const std::string& nameInShader,const unsigned int& programID,const unsigned int& uniformID)
{
    GLint loc = glGetUniformLocation(mPrograms[programID],nameInShader.c_str());
    if(loc < 0)
    {
        std::cout<<"Failed to get the location of the uniform variable '"<<nameInShader<<"' GL Error: "<<glGetError()<<std::endl;
    }
    mUniforms[uniformID] = loc;
}

void GLProgramManager::createUniformBlock(const unsigned int& programID,const std::string& nameInShader,const unsigned int& uniformBlockID)
{
    GLuint location = glGetUniformBlockIndex(mPrograms[programID],nameInShader.c_str());

    glUniformBlockBinding(mPrograms[programID],location,mLastUniformBindPoint);
    GLuint ubo;
    glGenBuffers(1,&ubo);
    glBindBufferBase(GL_UNIFORM_BUFFER,mLastUniformBindPoint,ubo);
    UniformBlock block;
    block.bindingPoint = mLastUniformBindPoint;
    block.uniformBuffer = ubo;
    mUniformBlocks[uniformBlockID] = block;
    mLastUniformBindPoint++;
}

void GLProgramManager::bindUniformBlock(const unsigned int& uniformBlockID)
{
    glBindBufferBase(GL_UNIFORM_BUFFER,mUniformBlocks[uniformBlockID].bindingPoint,mUniformBlocks[uniformBlockID].uniformBuffer);
}

void GLProgramManager::printShaderLog(GLuint shader)
{
    if(glIsShader(shader))
    {
        int infoLogLength = 0;
        int maxLength = 0;

        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&maxLength);

        char* infoLog = new char[maxLength];

        glGetShaderInfoLog(shader,maxLength,&infoLogLength,infoLog);
        if(infoLogLength > 0)
        {
            std::cout<<"Info log for shader "<<shader<<std::endl<<infoLog<<std::endl;
        }
        delete[] infoLog;
    }
    else
    {
        std::cout<<"Shader "<<shader<<" is not a valid shader object.\n";
    }
}

void GLProgramManager::printProgramLog(GLuint program)
{
    if(glIsProgram(program))
    {
        int infoLogLength = 0;
        int maxLength = 0;

        glGetProgramiv(program,GL_INFO_LOG_LENGTH,&maxLength);

        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(program,maxLength,&infoLogLength,infoLog);
        if(infoLogLength > 0)
        {
            std::cout<<"Info log for program "<<program<<"\n"<<infoLog<<std::endl;
        }
        delete[] infoLog;
    }
    else
    {
        std::cout<<"Program "<<program<<" is not a valid program object.\n";
    }
}

void GLProgramManager::deleteProgram(const unsigned int& programID)
{
    std::vector< std::list<GLShader>::iterator > toDelete;

    for(auto iter = mShaders.begin();iter != mShaders.end();iter++)
    {
        if(iter->program == mPrograms[programID])
        {
            toDelete.push_back(iter);
            glDetachShader(iter->program,iter->shader);
            glDeleteShader(iter->shader);
        }
    }

    glDeleteProgram(mPrograms[programID]);

    for(auto& iter: toDelete)
    {
        mShaders.erase(iter);
    }
    toDelete.clear();
}
