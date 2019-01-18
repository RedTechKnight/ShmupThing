cmake_minimum_required(VERSION 3.0.0)
project(shmup_linux VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories( "/usr/include/SDL2"| "/usr/include"| "Source")


add_executable(shmup_linux 
                            Source/main.cpp
                            Source/Game.cpp
                            Source/AudioSystem.cpp
                            Source/collision.cpp
                            Source/Cursor.cpp
                            Source/sdl_gl_window.cpp
                            Source/Enemy.cpp
                            Source/EnemyManager.cpp
                            Source/GLModel.cpp
                            Source/GLProgramManager.cpp
                            Source/GLTexture.cpp
                            Source/GL2DElement.cpp
                            Source/GL2DElementArray.cpp
                            Source/GLCamera.cpp
                            Source/GLModelManager.cpp
                            Source/GLMultilineText.cpp
                            Source/GLProjectileRenderer.cpp
                            Source/GLText.cpp
                            Source/GUIButton.cpp
                            Source/GUICheckBox.cpp
                            Source/GUISystem.cpp
                            Source/Input.cpp
                            Source/Particles.cpp
                            Source/Player.cpp
                            Source/Projectile.cpp
                            Source/ship.cpp
                            Source/Light.cpp
                             )


target_link_libraries(shmup_linux -lm  clang
                                    stdc++
                                    GLX_mesa
                                    GLEW
                                    SDL2main
                                    SDL2
                                    SDL2_mixer
                                    freeimage
                                    SDL2_ttf
                                    assimp)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

