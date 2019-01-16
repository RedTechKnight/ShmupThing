#include "GLModel.h"

GLModel::GLModel()
{
    mVertices.clear();
    mIndices.clear();
    mVertexAttributes.clear();
    mVertexBuffers.clear();
    mIndexBuffer = 0;
    mVertexArrayObject = 0;
    mVertexComponents = 0;
}

GLModel::~GLModel()
{
    mVertices.clear();
    mIndices.clear();
    mVertexAttributes.clear();
    mVertexBuffers.clear();
    mIndexBuffer = 0;
    mVertexArrayObject = 0;
}

void GLModel::addVertexAttribute(GLenum type,uint32_t num)
{
    mVertexComponents += num;
    element em;
    em.type = type;
    em.num = num;
    mVertexAttributes.push_back(em);
    std::vector<float> floatList;
    mVertices.push_back(floatList);
    //When a new vertex element is added, add a list of floats to the list in mVertices to store its componenets
}

void GLModel::addElement(float elem,uint32_t index)
{
    mVertices[index].push_back(elem);
}

void GLModel::addVec2(float x,float y,uint32_t index)
{
    if(mVertexAttributes[index].num < 2)
    {
        throw("Attempted to add 2 dimensional value to 1 dimensional attribute");
    }
    mVertices[index].push_back(x);
    mVertices[index].push_back(y);
    if(mVertexAttributes[index].num == 3)
    {
        mVertices[index].push_back(0.0f);
    }
    if(mVertexAttributes[index].num == 4)
    {
        mVertices[index].push_back(0.0f);
        mVertices[index].push_back(0.0f);
    }
}

void GLModel::addVec3(float x,float y,float z,uint32_t index)
{
    if(mVertexAttributes[index].num < 3)
    {
        throw("Attempted to add 3 dimensional value to 2 or 1 dimensional attribute");
    }
    mVertices[index].push_back(x);
    mVertices[index].push_back(y);
    mVertices[index].push_back(z);
    if(mVertexAttributes[index].num == 4)
    {
        mVertices[index].push_back(0.0f);
    }
}

void GLModel::addVec4(float x,float y,float z,float w,uint32_t index)
{
    if(mVertexAttributes[index].num < 4)
    {
        throw("Attempted to add 4 dimensional value to 3, 2 or 1 dimensional attribute");
    }
    mVertices[index].push_back(x);
    mVertices[index].push_back(y);
    mVertices[index].push_back(z);
    mVertices[index].push_back(w);
}
void GLModel::addIndex(uint32_t index)
{
    mIndices.push_back(index);
}

void GLModel::assImpLoad(const std::string& modelfile,const int& flags)
{
    Assimp::Importer importer{Assimp::Importer()};
    const aiScene *scene = importer.ReadFile(modelfile,flags);
    std::vector<aiNode*> visitedNodes;
    if(scene->mRootNode->mNumChildren > 0)
    {
        std::stack<aiNode*> nodeStack;
        aiNode* currentNode = scene->mRootNode;
        bool treeEnd = false;
        nodeStack.push(currentNode);
        while(!treeEnd)
        {
            //Lamda to compare current node to already visited nodes
            auto condition = [currentNode](aiNode* node) -> bool
            {
                return (currentNode == node);
            };

            if(!std::any_of(visitedNodes.begin(),visitedNodes.end(),condition))
            {
                visitedNodes.push_back(currentNode);
            }

            bool unvisitedChildren = false;
            for(unsigned int i = 0;i < currentNode->mNumChildren;i++)
            {
                aiNode* temp = currentNode->mChildren[i];
                auto condition2 = [temp](aiNode* node) -> bool
                {
                    return (temp == node);
                };

                if(!std::any_of(visitedNodes.begin(),visitedNodes.end(),condition2))
                {
                    nodeStack.push(temp);
                    unvisitedChildren = true;
                }
            }

            if(unvisitedChildren)
            {
                currentNode = nodeStack.top();
            }
            else
            {
                nodeStack.pop();
                if(nodeStack.size() > 1)
                {
                    currentNode = nodeStack.top();
                }
                else
                {
                    treeEnd = true;
                }
            }
        }
    }
    else
    {
        visitedNodes.push_back(scene->mRootNode);
    }

    std::vector<aiMesh*> meshes;

    for(auto n: visitedNodes)
    {
        for(unsigned int i = 0;i < n->mNumMeshes;++i)
        {
            meshes.push_back(scene->mMeshes[n->mMeshes[i]]);
        }
    }

    if(meshes[0]->mNumVertices > 0)
    {
        addVertexAttribute(GL_FLOAT,3);
    }
    if(meshes[0]->mNormals)
    {
        addVertexAttribute(GL_FLOAT,3);
    }
    if(meshes[0]->mTextureCoords[0])
    {
        addVertexAttribute(GL_FLOAT,2);
    }
    for(auto m: meshes)
    {
        for(unsigned int i = 0;i < m->mNumVertices;i++)
        {

            addVec3(m->mVertices[i].x,m->mVertices[i].z,m->mVertices[i].y,0);
            if(m->mNormals)
            {
                addVec3(m->mNormals[i].x,m->mNormals[i].z,m->mNormals[i].y,1);
            }
            if(m->mTextureCoords[0])
            {
                addVec2(m->mTextureCoords[0][i].x,m->mTextureCoords[0][i].y,2);
            }
        }
        for(unsigned int i = 0;i < m->mNumFaces;i++)
        {
            addIndex(m->mFaces[i].mIndices[0]);
            addIndex(m->mFaces[i].mIndices[1]);
            addIndex(m->mFaces[i].mIndices[2]);
        }
    }

}

void GLModel::createVertexBuffers()
{
    glGenVertexArrays(1,&mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);
    for(uint32_t i = 0;i < mVertexAttributes.size();i++)
    {
        unsigned int buffer = 0;
        glGenBuffers(1,&buffer);
        glBindBuffer(GL_ARRAY_BUFFER,buffer);
        mVertexBuffers.push_back(buffer);
        glBufferData(GL_ARRAY_BUFFER,mVertices[i].size()*sizeof(GLfloat),mVertices[i].data(),GL_STATIC_DRAW);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i,mVertexAttributes[i].num,mVertexAttributes[i].type,GL_FALSE,0,nullptr);
    }
}

void GLModel::createIndexBuffers()
{
    glGenBuffers(1,&mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,mIndices.size()*sizeof(GLuint),mIndices.data(),GL_STATIC_DRAW);
}

void GLModel::draw(GLenum primType,const unsigned int& instanceCount)
{
    glBindVertexArray(mVertexArrayObject);
    if(mIndices.size()>0)
    {

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mIndexBuffer);
        glDrawElementsInstanced(primType,mIndices.size(),GL_UNSIGNED_INT,nullptr,instanceCount);
    }
    else if(mVertices.size() > 0)
    {
        glDrawArrays(primType,0,mVertices[0].size()/mVertexComponents);
        glDrawArraysInstanced(primType,0,mVertices[0].size()/mVertexComponents,instanceCount);
    }
}

void GLModel::clean()
{
    for(uint32_t i = 0;i < mVertexBuffers.size();i++)
    {
       glDeleteBuffers(1,&mVertexBuffers[i]);
       mVertices[i].clear();
    }
    glDeleteBuffers(1,&mIndexBuffer);
    mIndices.clear();
    mVertexBuffers.clear();
    mVertices.clear();
}
