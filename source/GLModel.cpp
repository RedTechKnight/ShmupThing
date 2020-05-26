#include "../headers/GLModel.h"

GLModel::GLModel() {
  vertices.clear();
  indices.clear();
  vertex_attributes.clear();
  vertex_buffers.clear();
  index_buffer = 0;
  vertex_array_object = 0;
  vertex_components = 0;
}

GLModel::~GLModel() {
  vertices.clear();
  indices.clear();
  vertex_attributes.clear();
  vertex_buffers.clear();
  index_buffer = 0;
  vertex_array_object = 0;
}

void GLModel::add_vertex_attribute(GLenum type, uint32_t num) {
  vertex_components += num;
  Element em;
  em.type = type;
  em.num = num;
  vertex_attributes.push_back(em);
  std::vector<float> float_list;
  vertices.push_back(float_list);
  // When a new vertex element is added, add a list of floats to the list in
  // mVertices to store its componenets
}

void GLModel::add_element(float elem, uint32_t index) {
  vertices[index].push_back(elem);
}

void GLModel::add_vec_2(float x, float y, uint32_t index) {
  if (vertex_attributes[index].num < 2) {
    throw("Attempted to add 2 dimensional value to 1 dimensional attribute");
  }
  vertices[index].push_back(x);
  vertices[index].push_back(y);
  if (vertex_attributes[index].num == 3) {
    vertices[index].push_back(0.0f);
  }
  if (vertex_attributes[index].num == 4) {
    vertices[index].push_back(0.0f);
    vertices[index].push_back(0.0f);
  }
}

void GLModel::add_vec_3(float x, float y, float z, uint32_t index) {
  if (vertex_attributes[index].num < 3) {
    throw(
        "Attempted to add 3 dimensional value to 2 or 1 dimensional attribute");
  }
  vertices[index].push_back(x);
  vertices[index].push_back(y);
  vertices[index].push_back(z);
  if (vertex_attributes[index].num == 4) {
    vertices[index].push_back(0.0f);
  }
}

void GLModel::add_vec_4(float x, float y, float z, float w, uint32_t index) {
  if (vertex_attributes[index].num < 4) {
    throw("Attempted to add 4 dimensional value to 3, 2 or 1 dimensional "
          "attribute");
  }
  vertices[index].push_back(x);
  vertices[index].push_back(y);
  vertices[index].push_back(z);
  vertices[index].push_back(w);
}
void GLModel::add_index(uint32_t index) { indices.push_back(index); }

void GLModel::assimp_load(const std::string &model_file, const int &flags) {
  Assimp::Importer importer{};
  const aiScene *scene = importer.ReadFile(model_file, flags);
  std::vector<aiNode *> visited_nodes;
  if (scene->mRootNode->mNumChildren > 0) {
    std::stack<aiNode *> node_stack;
    aiNode *current_node = scene->mRootNode;
    bool tree_end = false;
    node_stack.push(current_node);
    while (!tree_end) {
      // Lamda to compare current node to already visited nodes
      auto condition = [current_node](aiNode *node) -> bool {
        return (current_node == node);
      };

      if (!std::any_of(visited_nodes.begin(), visited_nodes.end(), condition)) {
        visited_nodes.push_back(current_node);
      }

      bool unvisited_children = false;
      for (unsigned int i = 0; i < current_node->mNumChildren; i++) {
        aiNode *temp = current_node->mChildren[i];
        auto condition2 = [temp](aiNode *node) -> bool {
          return (temp == node);
        };

        if (!std::any_of(visited_nodes.begin(), visited_nodes.end(),
                         condition2)) {
          node_stack.push(temp);
          unvisited_children = true;
        }
      }

      if (unvisited_children) {
        current_node = node_stack.top();
      } else {
        node_stack.pop();
        if (node_stack.size() > 1) {
          current_node = node_stack.top();
        } else {
          tree_end = true;
        }
      }
    }
  } else {
    visited_nodes.push_back(scene->mRootNode);
  }

  std::vector<aiMesh *> meshes;

  for (auto n : visited_nodes) {
    for (unsigned int i = 0; i < n->mNumMeshes; ++i) {
      meshes.push_back(scene->mMeshes[n->mMeshes[i]]);
    }
  }

  if (meshes[0]->mNumVertices > 0) {
    add_vertex_attribute(GL_FLOAT, 3);
  }
  if (meshes[0]->mNormals) {
    add_vertex_attribute(GL_FLOAT, 3);
  }
  if (meshes[0]->mTextureCoords[0]) {
    add_vertex_attribute(GL_FLOAT, 2);
  }
  for (auto m : meshes) {
    for (unsigned int i = 0; i < m->mNumVertices; i++) {

      add_vec_3(m->mVertices[i].x, m->mVertices[i].z, m->mVertices[i].y, 0);
      if (m->mNormals) {
        add_vec_3(m->mNormals[i].x, m->mNormals[i].z, m->mNormals[i].y, 1);
      }
      if (m->mTextureCoords[0]) {
        add_vec_2(m->mTextureCoords[0][i].x, m->mTextureCoords[0][i].y, 2);
      }
    }
    for (unsigned int i = 0; i < m->mNumFaces; i++) {
      add_index(m->mFaces[i].mIndices[0]);
      add_index(m->mFaces[i].mIndices[1]);
      add_index(m->mFaces[i].mIndices[2]);
    }
  }
}

void GLModel::create_vertex_buffers() {
  glGenVertexArrays(1, &vertex_array_object);
  glBindVertexArray(vertex_array_object);
  for (uint32_t i = 0; i < vertex_attributes.size(); i++) {
    unsigned int buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    vertex_buffers.push_back(buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices[i].size() * sizeof(GLfloat),
                 vertices[i].data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, vertex_attributes[i].num,
                          vertex_attributes[i].type, GL_FALSE, 0, nullptr);
  }
}

void GLModel::create_index_buffers() {
  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);
}

void GLModel::draw(GLenum prim_type, const unsigned int &instance_count) {
  glBindVertexArray(vertex_array_object);
  if (indices.size() > 0) {

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glDrawElementsInstanced(prim_type, indices.size(), GL_UNSIGNED_INT, nullptr,
                            instance_count);
  } else if (vertices.size() > 0) {
    glDrawArrays(prim_type, 0, vertices[0].size() / vertex_components);
    glDrawArraysInstanced(prim_type, 0, vertices[0].size() / vertex_components,
                          instance_count);
  }
}

void GLModel::clean() {
  for (uint32_t i = 0; i < vertex_buffers.size(); i++) {
    glDeleteBuffers(1, &vertex_buffers[i]);
    vertices[i].clear();
  }
  glDeleteBuffers(1, &index_buffer);
  indices.clear();
  vertex_buffers.clear();
  vertices.clear();
}
