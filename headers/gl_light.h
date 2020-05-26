#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED
#include <GL/glew.h>
#include <array>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>
struct LightBlock {
  std::array<glm::vec4, 64> position;
  std::array<glm::vec4, 64> colour;
};

struct Light {
  glm::vec4 position;
  glm::vec4 colour;
};

class LightManager {
private:
  int light_count;

public:
  LightBlock lights;

  LightManager();
  ~LightManager();

  void get_lights(std::list<Light> &lights);
  void upload_data();
};

#endif // LIGHT_H_INCLUDED
