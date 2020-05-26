#include "../headers/glm_camera.h"

Camera::Camera()
    : view_matrix(glm::mat4()), projection_matrix(glm::mat4()),
      upright_matrix(1), right_vector(1, 0, 0), up_vector(0, 1, 0),
      look_vector(0, 0, 1), eye_position(0, 0, 0), target_position(0, 0, 0),
      is_orientation_updated(false), is_following_target(false) {}

Camera::~Camera() {
  view_matrix = glm::mat4();
  projection_matrix = glm::mat4();
}

void Camera::create_view_matrix(const glm::vec3 &eye_pos,
                                const glm::vec3 &view_vec) {
  eye_position = eye_pos;
  look_vector = glm::normalize(view_vec);

  view_matrix[0][0] = right_vector.x;
  view_matrix[1][0] = right_vector.y;
  view_matrix[2][0] = right_vector.z;
  view_matrix[3][0] = -glm::dot(eye_position, right_vector);

  view_matrix[0][1] = up_vector.x;
  view_matrix[1][1] = up_vector.y;
  view_matrix[2][1] = up_vector.z;
  view_matrix[3][1] = -glm::dot(eye_position, up_vector);

  view_matrix[0][2] = look_vector.x;
  view_matrix[1][2] = look_vector.y;
  view_matrix[2][2] = look_vector.z;
  view_matrix[3][2] = -glm::dot(eye_position, right_vector);

  view_matrix[0][3] = 0;
  view_matrix[1][3] = 0;
  view_matrix[2][3] = 0;
  view_matrix[3][3] = 1;
}

void Camera::follow_target(const glm::vec3 &target) {
  if (!is_following_target) {
    is_following_target = true;
  }
  target_position = target;
}

void Camera::recreate_view_matrix() {
  if (is_orientation_updated) {
    if (!is_following_target) {
      look_vector = upright_matrix * glm::vec3(0, 0, 1);
      right_vector = upright_matrix * glm::vec3(1, 0, 0);
      up_vector = upright_matrix * glm::vec3(0, 1, 0);
    }
    is_orientation_updated = false;
  }

  view_matrix[0][0] = right_vector.x;
  view_matrix[1][0] = right_vector.y;
  view_matrix[2][0] = right_vector.z;
  view_matrix[3][0] = -glm::dot(eye_position + target_position, right_vector);

  view_matrix[0][1] = up_vector.x;
  view_matrix[1][1] = up_vector.y;
  view_matrix[2][1] = up_vector.z;
  view_matrix[3][1] = -glm::dot(eye_position + target_position, up_vector);

  view_matrix[0][2] = look_vector.x;
  view_matrix[1][2] = look_vector.y;
  view_matrix[2][2] = look_vector.z;
  view_matrix[3][2] = -glm::dot(eye_position + target_position, look_vector);

  view_matrix[0][3] = 0;
  view_matrix[1][3] = 0;
  view_matrix[2][3] = 0;
  view_matrix[3][3] = 1;
}

void Camera::create_projection_matrix(const float &near_plane,
                                    const float &far_plane, const float &FOV,
                                    const float &aspect_ratio) {
  projection_matrix =
      glm::mat4(1.0f / (aspect_ratio * glm::tan(FOV / 2)), 0, 0, 0, 0,
                1.0f / glm::tan(FOV / 2), 0, 0, 0, 0,
                far_plane / (far_plane - near_plane), 1, 0, 0,
                -near_plane * far_plane / (far_plane - near_plane), 0);
}

void Camera::pitch(const float &angle) {

  glm::mat3 rot;
  rot = glm::rotate(glm::mat4(1), -angle, right_vector);
  upright_matrix = upright_matrix * rot;
  eye_position = rot * eye_position;
  look_vector = rot * look_vector;
  right_vector = rot * right_vector;
  up_vector = rot * up_vector;
  is_orientation_updated = true;
}

void Camera::heading(const float &angle) {
  glm::mat3 rot;
  rot = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0));
  upright_matrix = upright_matrix * rot;
  eye_position = rot * eye_position;
  look_vector = rot * look_vector;
  right_vector = rot * right_vector;
  up_vector = rot * up_vector;
  is_orientation_updated = true;
}

void Camera::move(const glm::vec3 &dir) {
  eye_position = eye_position + (upright_matrix * dir);
}

glm::mat3 Camera::get_upright_matrix() { return upright_matrix; }

glm::mat4 Camera::get_view_matrix() { return view_matrix; }

glm::mat4 Camera::get_projection_matrix() { return projection_matrix; }
