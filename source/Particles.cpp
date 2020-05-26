#include "../headers/Particles.h"

ParticleSystem::ParticleSystem()
    : next_emit_time(0), origin(0, 0, 0), max_size(0), emit_rate(0),
      emit_interval(0), is_looping(false), start_life(0) {
  particles.clear();
}

ParticleSystem::~ParticleSystem() { particles.clear(); }
void ParticleSystem::create() {
  std::fill(offsets.begin(), offsets.end(), glm::vec4(0, 0, 0, 0));
  if (emit_rate > max_size)
    emit_rate = max_size;

  for (unsigned int i = 0; i < emit_rate; i++) {
    Particle particle;
    particle.active = true;
    particle.life = start_life;
    particle.offset = origin;
    particle.velocity = random_function();
    particles.push_back(particle);
  }
}

void ParticleSystem::update(const float &delta) {
  effect_life -= delta;

  std::fill(offsets.begin(), offsets.end(), glm::vec4(0, 0, 0, 0));
  next_emit_time -= delta;
  if (next_emit_time <= 0) {
    next_emit_time = emit_interval;
    for (unsigned int i = 0;
         i < glm::min(max_size - static_cast<unsigned int>(particles.size()),
                      emit_rate);
         i++) {
      Particle particle;
      particle.active = true;
      particle.life = start_life;
      particle.offset = origin;
      particle.velocity = random_function();
      particles.push_back(particle);
    }
  }

  auto update_offset = [&delta](Particle &particle) -> void {
    particle.offset = particle.offset + particle.velocity * delta / 1000.0f;
    particle.life -= delta;
  };

  std::for_each(particles.begin(), particles.end(), update_offset);

  auto remove_condition = [](Particle &particle) -> bool {
    return (particle.life <= 0);
  };

  particles.erase(
      std::remove_if(particles.begin(), particles.end(), remove_condition),
      particles.end());

  float start = start_life;
  auto offset_pack = [&start](Particle &particle) -> glm::vec4 {
    return glm::vec4(particle.offset,
                     static_cast<float>(particle.life) / start);
  };

  std::transform(particles.begin(), particles.end(), offsets.begin(),
                 offset_pack);
}

void ParticleSystem::upload() {
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4) * 4096,
                  offsets.data());
}

ParticleRenderer::ParticleRenderer() : texture(GLTexture()) {
  particles_systems.clear();
  models.clear();
}

ParticleRenderer::~ParticleRenderer() {
  particles_systems.clear();
  models.clear();
}

void ParticleRenderer::addParticleEffect(
    const unsigned int &ID, const ParticleSystem &particle_effect) {
  GLModel model;
  model.add_vertex_attribute(GL_FLOAT, 4);
  model.add_vec_4(0, 0, 0, 0, 0);
  model.create_vertex_buffers();
  models[ID] = model;
  particles_systems[ID] = particle_effect;
}

void ParticleRenderer::update(const unsigned int &delta) {
  for (auto &system : particles_systems) {
    system.second.update(delta);
  }

  for (auto iter = particles_systems.begin();
       iter != particles_systems.end();) {
    if (iter->second.effect_life <= 0) {
      iter = particles_systems.erase(iter);
    } else {
      iter++;
    }
  }

  auto packLight =
      [](std::pair<const unsigned int, ParticleSystem> &system) -> Light {
    Light l;
    l.position = glm::vec4(system.second.origin + glm::vec3(0, 0, -10), 25);
    l.colour = glm::vec4(3, 0, 0, 1);
    return l;
  };

  lights.resize(particles_systems.size());
  std::transform(particles_systems.begin(), particles_systems.end(),
                 lights.begin(), packLight);
}

void ParticleRenderer::draw() {
  glEnable(GL_BLEND);
  // glEnable(GL_ALPHA_TEST);
  // glBlendFuncSeparate(GL_SRC_COLOR,GL_DST_COLOR,GL_SRC_ALPHA,GL_DST_ALPHA);
  // glBlendEquationSeparate(GL_FUNC_ADD,GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);
  texture.bind_texture(0);
  for (auto &system : particles_systems) {
    system.second.upload();
    models[system.first].draw(GL_POINTS, system.second.particles.size());
  }
  glDisable(GL_BLEND);
  // glDisable(GL_ALPHA_TEST);
}
