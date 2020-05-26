#ifndef PARTICLES_H_INCLUDED
#define PARTICLES_H_INCLUDED

#include "gl_model.h"
#include "gl_texture.h"
#include "gl_light.h"
#include <algorithm>
#include <functional>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>
#include <list>
#include <map>
#include <math.h>
#include <random>
#include <vector>

struct Particle {
  glm::vec3 offset;
  glm::vec3 velocity;
  bool active;
  int life;
};

class ParticleSystem {
private:
  int next_emit_time;

public:
  std::list<Particle> particles;
  std::array<glm::vec4, 4096> offsets;
  glm::vec3 origin;
  unsigned int max_size;
  unsigned int emit_rate;
  unsigned int emit_interval;
  bool is_looping;
  unsigned int start_life;
  int effect_life;
  std::function<glm::vec3()> random_function;

  ParticleSystem();
  ~ParticleSystem();

  void create();
  void update(const float &delta);
  void upload();
};

class ParticleRenderer {
private:
public:
  GLTexture texture;
  std::map<unsigned int, ParticleSystem> particles_systems;
  std::map<unsigned int, GLModel> models;
  std::list<Light> lights;

  ParticleRenderer();
  ~ParticleRenderer();

  void addParticleEffect(const unsigned int &ID, const ParticleSystem &system);
  void update(const unsigned int &delta);
  void draw();
};

#endif // PARTICLES_H_INCLUDED
