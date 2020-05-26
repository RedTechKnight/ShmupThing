project ('ShmupThing','cpp')
compiler = meson.get_compiler('cpp')
src_files = ['source/main.cpp',
'source/collision.cpp',
'source/enemy.cpp',
'source/enemy_manager.cpp',
'source/game.cpp',
'source/gl_2d_element.cpp',
'source/gl_2d_element_array.cpp',
'source/glm_camera.cpp',
'source/gl_model.cpp',
'source/gl_model_manager.cpp',
'source/gl_program_manager.cpp',
'source/gl_projectile_renderer.cpp',
'source/gl_texture.cpp',
'source/sdl_input.cpp',
'source/gl_light.cpp',
'source/gl_particles.cpp',
'source/player.cpp',
'source/gl_projectile.cpp',
'source/sdl_gl_window.cpp',
'source/ship.cpp']

glew = dependency('glew')
sdl2 = dependency('sdl2')
assimp = dependency('assimp')
fi = compiler.find_library('freeimage')

executable ('shmup',src_files,dependencies: [glew,sdl2,assimp,fi])
