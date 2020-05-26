#include "../headers/gl_light.h"

LightManager::LightManager() {
  for (unsigned int i = 0; i < 64; i++) {
    lights.position[i] = glm::vec4(0, 0, 0, 0);
    lights.colour[i] = glm::vec4(0, 0, 0, 0);
  }
}

LightManager::~LightManager() {
  for (unsigned int i = 0; i < 64; i++) {
    lights.position[i] = glm::vec4(0, 0, 0, 0);
    lights.colour[i] = glm::vec4(0, 0, 0, 0);
  }
}

void LightManager::get_lights(std::list<Light> &light_list) {

  for (auto &light : light_list) {
    if (light_count > 63) {
      break;
    }

    lights.position[light_count + 1] = light.position;
    lights.colour[light_count + 1] = light.colour;
    light_count++;
  }
  light_list.clear();
}

void LightManager::upload_data() {
  lights.colour[0].w = light_count + 1;
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightBlock), &lights);
  if (light_count > 0) {
    std::fill(lights.position.begin() + 1,
              lights.position.begin() + light_count + 1, glm::vec4(0, 0, 0, 0));
    std::fill(lights.colour.begin() + 1,
              lights.colour.begin() + light_count + 1, glm::vec4(0, 0, 0, 0));
    light_count = 0;
  }
}
