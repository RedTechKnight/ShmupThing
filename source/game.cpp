#include "../headers/game.h"
Game::Game()
    : window(GLWindow()), shader_program_manager(GLProgramManager()),
      main_camera(Camera()), input(InputHandler()),
      model_manager(GLModelManager()), total_time(0), cam_target(0, 0, 0),
      player(Player()), projectile_manager(ProjectileManager()),
      fire_event_handler(), hit_event_handler(), explosion_event_handler(),
      enemy_manager(EnemyManager()), particle_renderer(ParticleRenderer()),
      light_manager(LightManager()), menu_images(GL2DElementArray()),
      current_res(0), score(0),
      player_projectile_renderer(GLProjectileRenderer()),
      enemy_projectile_renderer(GLProjectileRenderer()), background(GLModel()),
      game_running(true), game_state(GameState::Initialising),
      menu_state(MenuState::LoadingMain) {}

Game::~Game() { game_running = false; }

void Game::start() {
  if (game_state == GameState::Initialising) {
    if (!window.create_window("SHMUP", 1280, 720, 4, 5)) {
      std::cout << "Window creation failed. Exiting\n";
      game_running = false;
      return;
    }
    glEnable(GL_TEXTURE_3D);
    shader_program_manager.create_program(ImageProgramID);
    shader_program_manager.load_shader("Assets/2delemvertex.glsl",
                                       GL_VERTEX_SHADER, ImageProgramID);
    shader_program_manager.load_shader("Assets/2delemgeometry.glsl",
                                       GL_GEOMETRY_SHADER, ImageProgramID);
    shader_program_manager.load_shader("Assets/2delemfragment.glsl",
                                       GL_FRAGMENT_SHADER, ImageProgramID);
    shader_program_manager.link_program(ImageProgramID);

    shader_program_manager.create_program(ImageArrayProgramID);
    shader_program_manager.load_shader("Assets/2delemarrayvertex.glsl",
                                       GL_VERTEX_SHADER, ImageArrayProgramID);
    shader_program_manager.load_shader("Assets/2delemarraygeometry.glsl",
                                       GL_GEOMETRY_SHADER, ImageArrayProgramID);
    shader_program_manager.load_shader("Assets/2delemarrayfragment.glsl",
                                       GL_FRAGMENT_SHADER, ImageArrayProgramID);
    shader_program_manager.link_program(ImageArrayProgramID);

    gl_2d_element_array.init();

    shader_program_manager.get_uniform("uTexture", ImageProgramID,
                                       ImageProgramSamplerID);
    shader_program_manager.get_uniform("uTexture", ImageArrayProgramID,
                                       ImageArrayProgramID);

    shader_program_manager.create_uniform_block(
        ImageArrayProgramID, "Rects", ImageArrayProgramUniformBlockID);
    shader_program_manager.create_uniform_block(ImageProgramID, "ImageRect",
                                                ImageProgramUniformBlockID);


    shader_program_manager.bind_uniform_block(ImageArrayProgramUniformBlockID);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 180, nullptr,
                 GL_DYNAMIC_DRAW);

    shader_program_manager.bind_uniform_block(ImageProgramUniformBlockID);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 8, nullptr,
                 GL_DYNAMIC_DRAW);

    glEnable(GL_DEPTH_TEST);

    input.add_action(SDLK_a, LEFT);
    input.add_action(SDLK_d, RIGHT);
    input.add_action(SDLK_w, UP);
    input.add_action(SDLK_s, DOWN);
    input.add_action(SDLK_LEFT, LEFT);
    input.add_action(SDLK_RIGHT, RIGHT);
    input.add_action(SDLK_UP, UP);
    input.add_action(SDLK_DOWN, DOWN);
    input.add_action(SDLK_LCTRL, FIRE);
    input.add_action(SDLK_SPACE, FIRE);
    input.add_action(SDLK_ESCAPE, QUIT);
    game_state = GameState::GameLoading;
  }
}

void Game::update_interface(const uint32_t &delta) {
  while (SDL_PollEvent(&sdl_evet_handle)) {
    if (sdl_evet_handle.type == SDL_KEYDOWN ||
        sdl_evet_handle.type == SDL_KEYUP) {
      input.handle_input(sdl_evet_handle);
    }
    if (input.is_key_just_pressed(QUIT) || input.is_key_held(QUIT)) {
      game_running = false;
    }
  }
}

void Game::init() {
  if (game_state == GameState::GameLoading) {
    cam_target = glm::vec3(0, 0, 0);
    main_camera.create_view_matrix(glm::vec3(0, 0, -50), glm::vec3(0, 0, 1));
    main_camera.create_projection_matrix(0.1f, 1000.0f, PI / 3, 16.0f / 9.0f);
    viewport.view = main_camera.get_view_matrix();
    viewport.projection = main_camera.get_projection_matrix();

    model_manager.batch_load("Assets/models.txt");

    shader_program_manager.create_program(DefaultProgramID);
    shader_program_manager.load_shader("Assets/defaultVertex.glsl",
                                       GL_VERTEX_SHADER, DefaultProgramID);
    shader_program_manager.load_shader("Assets/defaultFragment.glsl",
                                       GL_FRAGMENT_SHADER, DefaultProgramID);
    shader_program_manager.link_program(DefaultProgramID);

    shader_program_manager.create_program(ParticleProgramID);
    shader_program_manager.load_shader("Assets/particlesVertex.glsl",
                                       GL_VERTEX_SHADER, ParticleProgramID);
    shader_program_manager.load_shader("Assets/particlesGeometry.glsl",
                                       GL_GEOMETRY_SHADER, ParticleProgramID);
    shader_program_manager.load_shader("Assets/particlesFragment.glsl",
                                       GL_FRAGMENT_SHADER, ParticleProgramID);
    shader_program_manager.link_program(ParticleProgramID);

    shader_program_manager.use_program(DefaultProgramID);
    shader_program_manager.get_uniform("uTexture", ParticleProgramID,
                                       ParticleProgramSamplerID);

    shader_program_manager.create_uniform_block(
        DefaultProgramID, "viewParams", DefaultProgramUniformBlockViewID);
    shader_program_manager.create_uniform_block(
        ParticleProgramID, "viewport", ParticleProgramUniformBlockViewportID);
    shader_program_manager.create_uniform_block(
        ParticleProgramID, "particle", ParticleProgramUniformBlockParticleID);
    shader_program_manager.create_uniform_block(
        DefaultProgramID, "Light", DefaultProgramUniformBlockLightID);
    shader_program_manager.create_uniform_block(
        DefaultProgramID, "Offsets", DefaultProgramUniformBlockOffsetID);

    shader_program_manager.bind_uniform_block(DefaultProgramUniformBlockViewID);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Viewport), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Viewport::view),
                    sizeof(Viewport::projection), &viewport.projection);

    shader_program_manager.bind_uniform_block(
        ParticleProgramUniformBlockViewportID);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Viewport), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Viewport::projection),
                    sizeof(Viewport::view), &viewport.projection);

    shader_program_manager.bind_uniform_block(
        ParticleProgramUniformBlockParticleID);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 4096, nullptr,
                 GL_DYNAMIC_DRAW);

    shader_program_manager.bind_uniform_block(
        DefaultProgramUniformBlockLightID);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightBlock), nullptr,
                 GL_DYNAMIC_DRAW);

    shader_program_manager.bind_uniform_block(
        DefaultProgramUniformBlockOffsetID);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 512, nullptr,
                 GL_DYNAMIC_DRAW);

    player.id = 12;
    player.health = 4;
    player.model_id = player_model_id;
    player.position = glm::vec3(0, 0, 0);
    player.fire_delay = 100;

    enemy_manager.despawn_distance = 60;
    enemy_manager.spawn_distance = 60;

    projectile_manager.x_bounds = 60;
    projectile_manager.y_bounds = 60;

    particle_renderer.texture.load_from_file("Assets/explosion_effect.png",
                                             0);
    player_projectile_renderer.init();
    player_projectile_renderer.texture.load_from_file("Assets/plasma.png",
                                                      0);
    enemy_projectile_renderer.init();
    enemy_projectile_renderer.texture.load_from_file(
        "Assets/enemy_blast.png", 0);

    Enemy enemy;
    enemy.bounding_box = BoundingBox{glm::vec3(0, 0, 0), glm::vec3(2, 2, 2)};
    enemy.fire_rate = 2000;
    enemy.health = 100;
    enemy.hit_flash_length = 3503;
    enemy.milli_sec_step = 0.01f;
    enemy.model_id = 10;
    enemy.type = Type::Enemy;

    for (unsigned int i = 0; i < 10; i++) {
      enemy.id = 5 * i;
      enemy.position = glm::vec3(-30.0f + (5.0f * i), 20, 0);
      enemy.move_vel = glm::vec3(0.01, 0, 0);
      enemy.next_fire = 50 * i;
      enemy_manager.enemies.push_back(enemy);
      enemy.id = 55 * i;
      enemy.next_fire = 70 * i;
      enemy.position = glm::vec3(-30.0f + (5.0f * i), 10, 0);
      enemy.move_vel = glm::vec3(-.01, 0, 0);
      enemy_manager.enemies.push_back(enemy);
    }

    background.add_vertex_attribute(GL_FLOAT, 3);
    background.add_vertex_attribute(GL_FLOAT, 3);

    background.add_vec_3(-200, -200, 5, 0);
    background.add_vec_3(200, -200, 5, 0);
    background.add_vec_3(200, 200, 5, 0);
    background.add_vec_3(-200, 200, 5, 0);
    background.add_vec_3(0, 0, -1, 1);
    background.add_vec_3(0, 0, -1, 1);
    background.add_vec_3(0, 0, -1, 1);
    background.add_vec_3(0, 0, -1, 1);

    background.add_index(0);
    background.add_index(1);
    background.add_index(2);

    background.add_index(0);
    background.add_index(2);
    background.add_index(3);
    background.create_vertex_buffers();
    background.create_index_buffers();

    game_state = GameState::Playing;
  }
}

void Game::handle_events() {

  fire_event_handler.push_events(player.fire_events);
  fire_event_handler.push_events(enemy_manager.fire_events);

  hit_event_handler.push_events(enemy_manager.hit_events);
  hit_event_handler.push_events(player.hit_events);

  explosion_event_handler.push_events(enemy_manager.explosion_events);

  while (fire_event_handler.num_events() > 0) {
    ProjectileFireEvent last_event = fire_event_handler.get_last_event();
    Projectile proj;
    proj.bounds = last_event.bounds;
    proj.damage = last_event.damage;
    proj.model_id = last_event.model_id;
    proj.position = last_event.position;
    proj.type = last_event.type;
    proj.velocity = last_event.velocity;
    proj.hit = false;
    projectile_manager.projectiles.push_back(proj);
  }

  while (explosion_event_handler.num_events() > 0) {
    ExplosionEvent last_event = explosion_event_handler.get_last_event();
    ParticleSystem system;
    system.effect_life = last_event.effect_life;
    system.emit_interval = last_event.emit_interval;
    system.emit_rate = last_event.emit_rate;
    system.is_looping = false;
    system.max_size = last_event.max_size;
    system.origin = last_event.origin;
    auto func = []() -> glm::vec3 { return glm::sphericalRand(7.0f); };
    system.random_function = func;
    system.start_life = system.effect_life - 1;
    system.create();
    particle_renderer.addParticleEffect(last_event.ID, system);
    score += 1;

    SDL_Color col{255, 255, 255, 255};
  }
}

void Game::update(uint32_t delta) {
  if (game_state == GameState::Playing) {
    total_time += delta;

    handle_events();

    if (input.is_key_held(LEFT)) {
      player.position = player.position + glm::vec3(-0.091f * delta, 0, 0);
    }
    if (input.is_key_held(RIGHT)) {
      player.position = player.position + glm::vec3(0.091f * delta, 0, 0);
    }
    if (input.is_key_held(UP)) {
      player.position = player.position + glm::vec3(0, 0.091f * delta, 0);
    }
    if (input.is_key_held(DOWN)) {
      player.position = player.position + glm::vec3(0, -.091f * delta, 0);
    }
    if (input.is_key_held(FIRE)) {
      player.fire();
    }
    glm::vec3 &pos = player.position;
    if (pos.x > 50) {
      pos.x = 50;
    }
    if (pos.x < -50) {
      pos.x = -50;
    }
    if (pos.y > cam_target.y + 25) {
      pos.y = cam_target.y + 25;
    }
    if (pos.y < cam_target.y - 25) {
      pos.y = cam_target.y - 25;
    }

    player.update(delta);

    for (auto &enemy : enemy_manager.enemies) {
      int damage = projectile_manager.collide(enemy.type, enemy.bounding_box);
      enemy.health -= damage;
      if (damage > 0) {
        enemy.hit();
      }
    }

    int player_damage =
        projectile_manager.collide(player.type, player.bounding_box);
    if (player_damage > 0) {
      player.hit();
    }
    player.health -= player_damage;
    if (player.health <= 0) {
      game_running = false;
    }
    projectile_manager.screen_center = cam_target;
    projectile_manager.update(delta);
    enemy_manager.screen_center = cam_target;
    enemy_manager.player_location = player.position;
    enemy_manager.update(delta);

    main_camera.follow_target(cam_target);
    main_camera.recreate_view_matrix();

    particle_renderer.update(delta);

    light_manager.lights.position[0] =
        glm::vec4(cam_target + glm::vec3(30, 30, -20), 170);

    light_manager.get_lights(projectile_manager.lights);
    light_manager.get_lights(particle_renderer.lights);
  }
}

void Game::render() {
  if (game_state == GameState::Playing) {
    window.clearViewport(0.1, 0.1, 0.1, 1.0f);
    viewport.view = main_camera.get_view_matrix();
    shader_program_manager.use_program(DefaultProgramID);
    shader_program_manager.bind_uniform_block(DefaultProgramUniformBlockViewID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Viewport::view),
                    &viewport.view);
    shader_program_manager.bind_uniform_block(
        DefaultProgramUniformBlockLightID);
    light_manager.upload_data();

    shader_program_manager.bind_uniform_block(
        DefaultProgramUniformBlockOffsetID);
    glm::vec4 pos = glm::vec4(player.position, 1);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &pos);
    glm::vec4 colour = glm::vec4(player.hit_flash, 1, player.hit_flash, 1);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 256,
                    sizeof(glm::vec4), &colour);
    model_manager.draw(player.model_id, GL_TRIANGLES, 1);

    pos = glm::vec4(0, 0, 0, 1);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &pos);
    colour = glm::vec4(0, 0, 0, 1);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 256,
                    sizeof(glm::vec4), &colour);
    background.draw(GL_TRIANGLES, 1);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4) * 256,
                    enemy_manager.offsets.data());
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 256,
                    sizeof(glm::vec4) * 256, enemy_manager.hit_flashes.data());
    model_manager.draw(enemy_manager.enemies.begin()->model_id, GL_TRIANGLES,
                       enemy_manager.enemies.size());

    shader_program_manager.use_program(ParticleProgramID);
    shader_program_manager.bind_uniform_block(
        ParticleProgramUniformBlockViewportID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Viewport::view),
                    &viewport.view);

    shader_program_manager.bind_uniform_block(
        ParticleProgramUniformBlockParticleID);
    particle_renderer.draw();

    shader_program_manager.bind_uniform_block(
        ParticleProgramUniformBlockViewportID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Viewport::view),
                    &viewport.view);
    glProgramUniform1i(
        shader_program_manager.programs[ParticleProgramID],
        shader_program_manager.uniforms[ParticleProgramSamplerID], 0);
    shader_program_manager.bind_uniform_block(
        ParticleProgramUniformBlockParticleID);
    if (projectile_manager.player_offsets) {
      glBufferSubData(GL_UNIFORM_BUFFER, 0,
                      sizeof(glm::vec4) * projectile_manager.player_offsets,
                      projectile_manager.player_projectile_offsets.data());
      player_projectile_renderer.draw(projectile_manager.player_offsets);
    }
    if (projectile_manager.enemy_offsets) {
      glBufferSubData(GL_UNIFORM_BUFFER, 0,
                      sizeof(glm::vec4) * projectile_manager.enemy_offsets,
                      projectile_manager.enemy_projectile_offsets.data());
      enemy_projectile_renderer.draw(projectile_manager.enemy_offsets);
    }

    shader_program_manager.use_program(ImageArrayProgramID);
    glProgramUniform1i(
        shader_program_manager.programs[ImageArrayProgramID],
        shader_program_manager.uniforms[ImageArrayProgramSamplerID], 0);
    shader_program_manager.bind_uniform_block(ImageArrayProgramUniformBlockID);
    gl_2d_element_array.draw();
  }

  shader_program_manager.use_program(0);
  window.swapWindow();
}

void Game::close() { game_running = false; }
