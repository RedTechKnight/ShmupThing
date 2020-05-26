project ('ShmupThing','cpp')
compiler = meson.get_compiler('cpp')
src_files = ['Source/main.cpp',
'Source/collision.cpp',
'Source/Enemy.cpp',
'Source/EnemyManager.cpp',
'Source/Game.cpp',
'Source/GL2DElement.cpp',
'Source/GL2DElementArray.cpp',
'Source/GLCamera.cpp',
'Source/GLModel.cpp',
'Source/GLModelManager.cpp',
'Source/GLProgramManager.cpp',
'Source/GLProjectileRenderer.cpp',
'Source/GLTexture.cpp',
'Source/Input.cpp',
'Source/Light.cpp',
'Source/Particles.cpp',
'Source/Player.cpp',
'Source/Projectile.cpp',
'Source/sdl_gl_window.cpp',
'Source/ship.cpp']

glew = dependency('glew')
sdl2 = dependency('sdl2')
assimp = dependency('assimp')
fi = compiler.find_library('freeimage')

executable ('shmup',src_files,dependencies: [glew,sdl2,assimp,fi])
