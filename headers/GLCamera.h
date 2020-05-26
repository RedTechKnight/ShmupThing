#ifndef _GL_CAMERA_H
#define _GL_CAMERA_H
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>

class Camera {
private:
  glm::mat4 view_matrix;
  glm::mat4 projection_matrix;
  glm::mat3 upright_matrix;
  glm::vec3 right_vector;
  glm::vec3 up_vector;
  glm::vec3 look_vector;
  glm::vec3 eye_position;
  glm::vec3 target_position;
  bool is_orientation_updated;
  bool is_following_target;

public:
  Camera();
  ~Camera();

  void create_view_matrix(const glm::vec3 &eyePos, const glm::vec3 &viewDir);
  void follow_target(const glm::vec3 &target);
  void recreate_view_matrix();
  void create_projection_matrix(const float &nearPlane, const float &farPlane,
                                const float &FOV, const float &aspectRatio);
  glm::mat4 get_view_matrix();
  glm::mat4 get_projection_matrix();
  glm::mat3 get_upright_matrix();

  void heading(const float &angle);
  void pitch(const float &angle);
  void move(const glm::vec3 &dir);
};

#endif // _GL_CAMERA_H
