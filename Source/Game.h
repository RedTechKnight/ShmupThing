#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <string>
#include <iostream>
#include "Dragon_SDLGL.h"
#include "GL_ProgramManager.h"
#include "GL_Texture.h"
#include "GL_Model.h"
#include <glm.hpp>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include "AudioSystem.h"
#include "GLCamera.h"
#include "GLTextRenderer.h"
#include "ship.h"
#include "Player.h"
#include "Enemy.h"
#include "Input.h"
#include "Level.h"
#include "GLModelManager.h"
#include "GameEvent.h"
#include "AudioSystem.h"
#include "ResourceLoader.h"
#include "EnemyManager.h"
#include "Particles.h"
#include "Light.h"
#include "GL2DElement.h"
#include "Cursor.h"
#include "GL2DElementArray.h"
#include "GUIElement.h"
#include "GUIButton.h"
#include "GUICheckBox.h"
#include "GLText.h"
#include "GLProjectileRenderer.h"
const float PI = glm::pi<float>();
const unsigned int gPlayerModelID = 5;
const unsigned int gEnemyModelID = 10;
const unsigned int gProjectileModelID = 15;

struct Viewport
{
    glm::mat4 view;
    glm::mat4 projection;
};

enum class GameState
{
    Initialising,
    Menu,
    GameLoading,
    Playing
};

enum class MenuState
{
    LoadingMain,
    Main,
    LoadingSettings,
    Settings,
    LoadingPause,
    Pause
};
class Game
{
private:
    DragonGL mOpenGL;
    GLProgramManager mShaderProgramManager;
    Camera mMainCamera;
    SDL_Event mEventHandle;
    InputHandler mInput;
    GLModelManager mModelManager;
    uint32_t mTotalTime;
    glm::vec3 mCamTarget;
    Level mLevel;
    Player mPlayer;
    ProjectileManager mProjectileManager;
    GameEventHandler<ProjectileFireEvent> mFireEventHandler;
    GameEventHandler<HitEvent> mHitEventHandler;
    GameEventHandler<ExplosionEvent> mExplosionEventHandler;
    MixerAudio mAudio;
    EnemyManager mEnemyManager;
    ResourceLoader mResourceLoader;
    ParticleRenderer mParticleRenderer;
    Viewport mViewport;
    LightManager mLightManager;
    GL2DElement mCursorImage;
    Cursor mCursor;
    GL2DElementArray m2DElemArray;
    GUISystem mGUI;
    GLTextManager mTextManager;
    GUISystem mMenu;
    GLTextManager mMenuText;
    GL2DElementArray mMenuImages;
    GLProjectileRenderer mPlayerProjectileRenderer;
    GLProjectileRenderer mEnemyProjectileRenderer;
    GLModel mBackground;
    unsigned int mCurrentRes;
public:
    bool mGameRunning;
    GameState mState;
    MenuState mMenuState;
    Game();
    ~Game();
    void start();
    void init();
    void updateInterface(const uint32_t& delta);
    void renderInterface();
    void update(uint32_t delta);
    void render();

private:
    void close();
    void handleEvents();

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
};

/*
*/
#endif // GAME_H_INCLUDED
