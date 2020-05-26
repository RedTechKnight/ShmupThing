project ('ShmupThing','cpp')
compiler = meson.get_compiler('cpp')
src_files = ['source/main.cpp',
'source/collision.cpp',
'source/Enemy.cpp',
'source/EnemyManager.cpp',
'source/Game.cpp',
'source/GL2DElement.cpp',
'source/GL2DElementArray.cpp',
'source/GLCamera.cpp',
'source/GLModel.cpp',
'source/GLModelManager.cpp',
'source/GLProgramManager.cpp',
'source/GLProjectileRenderer.cpp',
'source/GLTexture.cpp',
'source/Input.cpp',
'source/Light.cpp',
'source/Particles.cpp',
'source/Player.cpp',
'source/Projectile.cpp',
'source/sdl_gl_window.cpp',
'source/ship.cpp']

glew = dependency('glew')
sdl2 = dependency('sdl2')
assimp = dependency('assimp')
fi = compiler.find_library('freeimage')

executable ('shmup',src_files,dependencies: [glew,sdl2,assimp,fi])
