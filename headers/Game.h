#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include "Enemy.h"
#include "EnemyManager.h"
#include "GL2DElementArray.h"
#include "GLCamera.h"
#include "GLModel.h"
#include "GLModelManager.h"
#include "GLProgramManager.h"
#include "GLProjectileRenderer.h"
#include "GLTexture.h"
#include "Input.h"
#include "Light.h"
#include "Particles.h"
#include "Player.h"
#include "sdl_gl_window.h"
#include "ship.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
const float PI = glm::pi<float>();
const unsigned int player_model_id = 5;
const unsigned int gEnemyModelID = 10;
const unsigned int gProjectileModelID = 15;

struct Viewport {
  glm::mat4 view;
  glm::mat4 projection;
};

enum class GameState { Initialising, Menu, GameLoading, Playing };

enum class MenuState {
  LoadingMain,
  Main,
  LoadingSettings,
  Settings,
  LoadingPause,
  Pause,
  LoadingFail,
  Fail
};
class Game {
private:
  GLWindow window;
  GLProgramManager shader_program_manager;
  Camera main_camera;
  SDL_Event sdl_evet_handle;
  InputHandler input;
  GLModelManager model_manager;
  uint32_t total_time;
  glm::vec3 cam_target;
  Player player;
  ProjectileManager projectile_manager;
  GameEventHandler<ProjectileFireEvent> fire_event_handler;
  GameEventHandler<HitEvent> hit_event_handler;
  GameEventHandler<ExplosionEvent> explosion_event_handler;
  EnemyManager enemy_manager;
  ParticleRenderer particle_renderer;
  Viewport viewport;
  LightManager light_manager;
  GL2DElementArray gl_2d_element_array;
  GL2DElementArray menu_images;
  GLProjectileRenderer player_projectile_renderer;
  GLProjectileRenderer enemy_projectile_renderer;
  GLModel background;
  unsigned int current_res;
  unsigned int score;
  unsigned int mMaxScore;

public:
  bool game_running;
  GameState game_state;
  MenuState menu_state;
  Game();
  ~Game();
  void start();
  void init();
  void update_interface(const uint32_t &delta);

  void update(uint32_t delta);
  void render();

private:
  void close();
  void handle_events();

  const unsigned int TextProgramID = 1;
  const unsigned int ImageProgramID = 2;
  const unsigned int ImageArrayProgramID = 3;
  const unsigned int DefaultProgramID = 4;
  const unsigned int ParticleProgramID = 5;

  const unsigned int TextProgramSamplerID = 1;
  const unsigned int ImageProgramSamplerID = 2;
  const unsigned int ImageArrayProgramSamplerID = 3;
  const unsigned int ParticleProgramSamplerID = 4;

  const unsigned int TextProgramUniformBlockID = 1;
  const unsigned int ImageProgramUniformBlockID = 2;
  const unsigned int ImageArrayProgramUniformBlockID = 3;
  const unsigned int DefaultProgramUniformBlockViewID = 4;
  const unsigned int DefaultProgramUniformBlockLightID = 5;
  const unsigned int DefaultProgramUniformBlockOffsetID = 6;
  const unsigned int ParticleProgramUniformBlockViewportID = 7;
  const unsigned int ParticleProgramUniformBlockParticleID = 8;

  const unsigned int LEFT = 1;
  const unsigned int RIGHT = 2;
  const unsigned int UP = 4;
  const unsigned int DOWN = 8;
  const unsigned int FIRE = 16;
  const unsigned int QUIT = 32;
};

#endif
