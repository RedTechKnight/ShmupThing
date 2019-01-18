#include "Game.h"
Game::Game() : mWindow(GLWindow()), mShaderProgramManager(GLProgramManager()),  mMainCamera(Camera()),
 mInput(InputHandler()), mModelManager(GLModelManager()),
mTotalTime(0),  mCamTarget(0,0,0), mPlayer(Player()),
mProjectileManager(ProjectileManager()),
mFireEventHandler(),mHitEventHandler(),mExplosionEventHandler(),
mAudio(MixerAudio()),   mEnemyManager(EnemyManager()),
mParticleRenderer(ParticleRenderer()),   mLightManager(LightManager()),
mCursorImage(GL2DElement()), mCursor(GUICursor()),
m2DElemArray(GL2DElementArray()),mGUI(GUISystem()),mTextManager(GLTextManager()),
mMenu(GUISystem()),mMenuText(GLTextManager()),mMenuImages(GL2DElementArray()),
mCurrentRes(0),mScore(0),mPlayerProjectileRenderer(GLProjectileRenderer()),
mEnemyProjectileRenderer(GLProjectileRenderer()),mBackground(GLModel()),
mGameRunning(true),mState(GameState::Initialising),mMenuState(MenuState::LoadingMain)
{

}

Game::~Game()
{
    mGameRunning = false;
}

void Game::start()
{
    if(mState == GameState::Initialising)
    {
        if(!mWindow.createWindow("SHMUP",1280,720,4,5))
        {
            std::cout<<"Window creation failed. Exiting\n";
            mGameRunning = false;
            return;
        }
        glEnable(GL_TEXTURE_3D);
        mShaderProgramManager.createProgram(TextProgramID);
        mShaderProgramManager.loadShader("../Assets/textVertex.glsl",GL_VERTEX_SHADER,TextProgramID);
        mShaderProgramManager.loadShader("../Assets/textGeometry.glsl",GL_GEOMETRY_SHADER,TextProgramID);
        mShaderProgramManager.loadShader("../Assets/textFragment.glsl",GL_FRAGMENT_SHADER,TextProgramID);
        mShaderProgramManager.linkProgram(TextProgramID);

        mShaderProgramManager.createProgram(ImageProgramID);
        mShaderProgramManager.loadShader("../Assets/2delemvertex.glsl",GL_VERTEX_SHADER,ImageProgramID);
        mShaderProgramManager.loadShader("../Assets/2delemgeometry.glsl",GL_GEOMETRY_SHADER,ImageProgramID);
        mShaderProgramManager.loadShader("../Assets/2delemfragment.glsl",GL_FRAGMENT_SHADER,ImageProgramID);
        mShaderProgramManager.linkProgram(ImageProgramID);

        mShaderProgramManager.createProgram(ImageArrayProgramID);
        mShaderProgramManager.loadShader("../Assets/2delemarrayvertex.glsl",GL_VERTEX_SHADER,ImageArrayProgramID);
        mShaderProgramManager.loadShader("../Assets/2delemarraygeometry.glsl",GL_GEOMETRY_SHADER,ImageArrayProgramID);
        mShaderProgramManager.loadShader("../Assets/2delemarrayfragment.glsl",GL_FRAGMENT_SHADER,ImageArrayProgramID);
        mShaderProgramManager.linkProgram(ImageArrayProgramID);

        m2DElemArray.init();

        mMenuImages.init();

        SDL_Color textColor = {255,0,0,255};

        mTextManager.setFont("../Assets/liberation-sans-regular.ttf",16);

        textColor = {255,255,255,255};
        mMenuText.setFont("../Assets/liberation-sans-regular.ttf",20);
        mMenuText.init();

        mCursorImage.init();
        mCursorImage.mTexture.loadFromFile("../Assets/cursor.png",0);
        mCursorImage.setTextureArea(0,0,1,1);

        mCursor.mScreenHeight = 720.0f;
        mCursor.mScreenWidth = 1280.0f;

        mShaderProgramManager.getUniform("uTexture",ImageProgramID,ImageProgramSamplerID);
        mShaderProgramManager.getUniform("uTextArray",TextProgramID,TextProgramSamplerID);
        mShaderProgramManager.getUniform("uTexture",ImageArrayProgramID,ImageArrayProgramID);

        mShaderProgramManager.createUniformBlock(TextProgramID,"TextParamList",TextProgramUniformBlockID);
        mShaderProgramManager.createUniformBlock(ImageArrayProgramID,"Rects",ImageArrayProgramUniformBlockID);
        mShaderProgramManager.createUniformBlock(ImageProgramID,"ImageRect",ImageProgramUniformBlockID);

        mShaderProgramManager.bindUniformBlock(TextProgramUniformBlockID);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(float)*220,nullptr,GL_DYNAMIC_DRAW);

        mShaderProgramManager.bindUniformBlock(ImageArrayProgramUniformBlockID);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(float)*180,nullptr,GL_DYNAMIC_DRAW);

        mShaderProgramManager.bindUniformBlock(ImageProgramUniformBlockID);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(float)*8,nullptr,GL_DYNAMIC_DRAW);

        glEnable(GL_DEPTH_TEST);

        mInput.addAction(SDLK_a,LEFT);
        mInput.addAction(SDLK_d,RIGHT);
        mInput.addAction(SDLK_w,UP);
        mInput.addAction(SDLK_s,DOWN);
        mInput.addAction(SDLK_LEFT,LEFT);
        mInput.addAction(SDLK_RIGHT,RIGHT);
        mInput.addAction(SDLK_UP,UP);
        mInput.addAction(SDLK_DOWN,DOWN);
        mInput.addAction(SDLK_LCTRL,FIRE);
        mInput.addAction(SDLK_SPACE,FIRE);
        mInput.addAction(SDLK_ESCAPE,QUIT);
        mState = GameState::Menu;
        mMenuState = MenuState::LoadingMain;
    }
}

void Game::updateInterface(const uint32_t& delta)
{
    if(mState == GameState::Menu)
    {
        if(mMenuState == MenuState::LoadingMain)
        {
            mMenu.mButtons.clear();
            mMenu.mCheckboxes.clear();
            mMenuText.reset();
            mMenuImages.reset();
            GUIButton button;
            button.mID = 0;
            button.mHeight = .09;
            button.mWidth = .16;
            button.mX = .4;
            button.mY = .2;
            button.mType = GUIElementType::Text;
            mMenu.mButtons.push_back(button);

            button.mID = 1;
            button.mY = .3;
            button.mWidth = .14;
            mMenu.mButtons.push_back(button);

            button.mID = 2;
            button.mY = .4;
            button.mWidth = .09;
            mMenu.mButtons.push_back(button);

            button.mID = 3;
            button.mY = .5f;
            button.mWidth = .07;
            mMenu.mButtons.push_back(button);
            SDL_Color col = {255,255,255,255};
            mMenuText.addText("Start Game",0,0,0,0,.1,col);
            mMenuText.addText("Settings",0,0,0,0,.1,col);
            mMenuText.addText("About",0,0,0,0,.1,col);
            mMenuText.addText("Quit",0,0,0,0,.1,col);
            for(auto& btt: mMenu.mButtons)
            {
                if(btt.mType == GUIElementType::Text)
                {

                    mMenuText.setTextPosition(btt.mID,btt.mX,btt.mY,
                                          btt.mWidth,btt.mHeight,.1f);
                }
                else if(btt.mType == GUIElementType::Image)
                {
                    mMenuImages.setElementRect(btt.mID,btt.mX,btt.mY,
                                               btt.mWidth,btt.mHeight);
                    mMenuImages.setTextureArea(btt.mID,0,0,1,1);
                    mMenuImages.setElementDepth(btt.mID,.1f);
                }
            }
            mMenuState = MenuState::Main;
        }


        if(mMenuState == MenuState::Main)
        {
            mMenu.update(mCursor.mX,mCursor.mY);
            for(auto& btt: mMenu.mButtons)
            {
                if(btt.mType == GUIElementType::Text)
                {
                    mMenuText.setColour(btt.mID,1,1,1);
                    if(btt.mActive)
                    {
                        mMenuText.setColour(btt.mID,1,0,0);
                    }
                    if(btt.mClicked)
                    {
                        mMenuText.setColour(btt.mID,0,1,0);
                    }
                }
                else if(btt.mType == GUIElementType::Image)
                {
                    if(btt.mActive)
                    {
                        mMenuImages.setElementRect(btt.mID,btt.mX,btt.mY,btt.mWidth+10,
                                                   btt.mHeight+10);
                    }
                    if(btt.mClicked)
                    {
                        mMenuImages.setElementRect(btt.mID,btt.mX,btt.mY,btt.mWidth-5,
                                                   btt.mHeight-5);
                    }
                }

                if(btt.mClicked)
                {
                    if(btt.mID == 0)
                    {
                        mState = GameState::GameLoading;
                    }
                    if(btt.mID == 1)
                    {
                        mMenuState = MenuState::LoadingSettings;
                    }
                    if(btt.mID == 3)
                    {
                        mGameRunning = false;
                    }
                }
            }
        }
        if(mMenuState == MenuState::LoadingSettings)
        {
            mMenu.mButtons.clear();
            mMenu.mCheckboxes.clear();
            mMenuText.reset();
            mMenuImages.reset();
            SDL_Color col = {255,255,255,255};
            SDL_DisplayMode mode = mWindow.mDisplayModes[0][mCurrentRes];
            std::string res = std::to_string(mode.w) + " x " + std::to_string(mode.h);
            mMenuText.addText("Resolution: ",.1f,.1f,.2f,.05f,.1f,col);
            mMenuText.addText("Windowed: ",.1f,.2f,.15,.05f,.1f,col);
            mMenuText.addText("Quality: ",.1f,.3f,.1,.05f,.1f,col);
            mMenuText.addText("Back: ",0,0,0,0,.4f,col);
            mMenuText.addText(res,.6,.1,.1,.05,.1,col);
            mMenuText.addText("Apply",0,0,0,0,.1,col);

            mMenuImages.addElement("../Assets/left_arrow.png");
            mMenuImages.addElement("../Assets/right_arrow.png");
            mMenuImages.addElement("../Assets/checkbox.png");

            GUIButton button;
            button.mID = 3;
            button.mHeight = .05f;
            button.mWidth = .1f;
            button.mX = .1f;
            button.mY = .5f;
            button.mType = GUIElementType::Text;
            mMenu.mButtons.push_back(button);
            button.mHeight = .05;
            button.mWidth = .1f;
            button.mX = .8f;
            button.mY = .1f;
            button.mID = 5;
            mMenu.mButtons.push_back(button);

            button.mID = 0;
            button.mHeight = .05f;
            button.mWidth = .05f;
            button.mX = .5;
            button.mY = .1;
            button.mType = GUIElementType::Image;
            mMenu.mButtons.push_back(button);

            button.mID = 1;
            button.mX = .71;
            mMenu.mButtons.push_back(button);

            GUICheckBox checkbox;
            checkbox.mID = 2;
            checkbox.mHeight = .05;
            checkbox.mWidth = .05;
            checkbox.mType = GUIElementType::Image;
            checkbox.mX = .5;
            checkbox.mY = .2;
            checkbox.mChecked = mWindow.isFullscreen();
            mMenu.mCheckboxes.push_back(checkbox);
            for(auto& btt: mMenu.mButtons)
            {
                if(btt.mType == GUIElementType::Text)
                {
                    mMenuText.setTextPosition(btt.mID,btt.mX,btt.mY,
                                          btt.mWidth,btt.mHeight,.1f);
                }

                if(btt.mType == GUIElementType::Image)
                {
                    mMenuImages.setElementRect(btt.mID,btt.mX,btt.mY,
                                               btt.mWidth,btt.mHeight);
                    mMenuImages.setElementDepth(btt.mID,.1f);
                    mMenuImages.setTextureArea(btt.mID,0,.5f,1,.5f);
                }
            }

            for(auto& cbx: mMenu.mCheckboxes)
            {
                if(cbx.mType == GUIElementType::Image)
                {
                    mMenuImages.setElementRect(cbx.mID,cbx.mX,cbx.mY,
                                               cbx.mWidth,cbx.mHeight);
                    mMenuImages.setElementDepth(cbx.mID,.1f);
                    if(cbx.mChecked)
                        mMenuImages.setTextureArea(cbx.mID,0,.5f,1,.5f);
                    else
                        mMenuImages.setTextureArea(cbx.mID,0,0,1,.5f);
                }
            }
            mMenuState = MenuState::Settings;
        }

        if(mMenuState == MenuState::Settings)
        {
            SDL_DisplayMode mode = mWindow.mDisplayModes[0][mCurrentRes];
            std::string resolution = "";
            resolution.append(std::to_string(mode.w));
            resolution.append(" x ");
            resolution.append(std::to_string(mode.h));
            SDL_Color col = {255,255,255,255};
            mMenuText.setText(4,resolution,col);
            mMenu.update(mCursor.mX,mCursor.mY);
            for(auto& btt: mMenu.mButtons)
            {
                if(btt.mType == GUIElementType::Text)
                {
                    mMenuText.setColour(btt.mID,1,1,1);
                    if(btt.mActive)
                    {
                        mMenuText.setColour(btt.mID,1,0,0);
                    }
                    if(btt.mClicked)
                    {
                        mMenuText.setColour(btt.mID,0,1,0);
                    }
                }

                else if(btt.mType == GUIElementType::Image)
                {
                    mMenuImages.setTextureArea(btt.mID,0,.5,1,.5);
                    if(btt.mActive)
                    {
                        mMenuImages.setTextureArea(btt.mID,0,0,1,.5);
                    }
                    if(btt.mClicked)
                    {

                    }
                }

                if(btt.mClicked)
                {
                    if(btt.mID == 3)
                    {
                        mMenuState = MenuState::LoadingMain;
                    }
                    if(btt.mID == 0)
                    {
                        if(mCurrentRes > 0)
                            mCurrentRes--;
                    }
                    if(btt.mID == 1)
                    {
                        if(mCurrentRes < mWindow.mDisplayModes[0].size()-1)
                            mCurrentRes++;
                    }
                    if(btt.mID == 5)
                    {
                        mWindow.setFullscreen(mMenu.mCheckboxes[0].mChecked);
                        mWindow.setDisplayMode(mWindow.mDisplayModes[0][mCurrentRes]);
                        mCursor.mScreenWidth = mWindow.mDisplayModes[0][mCurrentRes].w;
                        mCursor.mScreenHeight = mWindow.mDisplayModes[0][mCurrentRes].h;
                        mMenuImages.mScreenWidth = mWindow.mDisplayModes[0][mCurrentRes].w;
                        mMenuImages.mScreenHeight = mWindow.mDisplayModes[0][mCurrentRes].h;
                        mMenuText.mScreenWidth = mWindow.mDisplayModes[0][mCurrentRes].w;
                        mMenuText.mScreenHeight = mWindow.mDisplayModes[0][mCurrentRes].h;
                        glViewport(0,0,mWindow.mDisplayModes[0][mCurrentRes].w,mWindow.mDisplayModes[0][mCurrentRes].h);
                    }
                }
            }
            for(auto& cbx: mMenu.mCheckboxes)
            {
                if(cbx.mChecked)
                {
                    mMenuImages.setTextureArea(cbx.mID,0,.5,1,.5f);
                }
                else
                {
                    mMenuImages.setTextureArea(cbx.mID,0,0,1,.5f);
                }
            }
        }
    }

    while(SDL_PollEvent(&mEventHandle))
    {
        if(mEventHandle.type == SDL_KEYDOWN || mEventHandle.type == SDL_KEYUP)
        {
            mInput.handleInput(mEventHandle);
        }
        if(mInput.keyJustPressed(QUIT) || mInput.keyHeld(QUIT))
        {
            mGameRunning = false;
        }

        if(mEventHandle.type == SDL_MOUSEMOTION)
        {
            mCursor.move(mEventHandle.motion.x,mEventHandle.motion.y);
        }
        if(mEventHandle.type == SDL_MOUSEBUTTONDOWN)
        {
            if(mEventHandle.button.button == SDL_BUTTON_LEFT)
            {
                mMenu.mClicked = true;
            }
        }
    }
    mCursorImage.setElementPosition(mCursor.mX,mCursor.mY,.03f,.07f);
}

void Game::init()
{
    if(mState == GameState::GameLoading)
    {
        mCamTarget = glm::vec3(0,0,0);
        mMainCamera.createViewMatrix(glm::vec3(0,0,-50),glm::vec3(0,0,1));
        mMainCamera.createProjectionMatrix(0.1f,1000.0f,PI/3,16.0f/9.0f);
        mViewport.view = mMainCamera.getViewMatrix();
        mViewport.projection = mMainCamera.getProjectionMatrix();

        mModelManager.batchLoad("../Assets/models.txt");

        mShaderProgramManager.createProgram(DefaultProgramID);
        mShaderProgramManager.loadShader("../Assets/defaultVertex.glsl",GL_VERTEX_SHADER,DefaultProgramID);
        mShaderProgramManager.loadShader("../Assets/defaultFragment.glsl",GL_FRAGMENT_SHADER,DefaultProgramID);
        mShaderProgramManager.linkProgram(DefaultProgramID);

        mShaderProgramManager.createProgram(ParticleProgramID);
        mShaderProgramManager.loadShader("../Assets/particlesVertex.glsl",GL_VERTEX_SHADER,ParticleProgramID);
        mShaderProgramManager.loadShader("../Assets/particlesGeometry.glsl",GL_GEOMETRY_SHADER,ParticleProgramID);
        mShaderProgramManager.loadShader("../Assets/particlesFragment.glsl",GL_FRAGMENT_SHADER,ParticleProgramID);
        mShaderProgramManager.linkProgram(ParticleProgramID);

        mShaderProgramManager.useProgram(DefaultProgramID);
        mShaderProgramManager.getUniform("uTexture",ParticleProgramID,ParticleProgramSamplerID);

        mShaderProgramManager.createUniformBlock(DefaultProgramID,"viewParams",DefaultProgramUniformBlockViewID);
        mShaderProgramManager.createUniformBlock(ParticleProgramID,"viewport",ParticleProgramUniformBlockViewportID);
        mShaderProgramManager.createUniformBlock(ParticleProgramID,"particle",ParticleProgramUniformBlockParticleID);
        mShaderProgramManager.createUniformBlock(DefaultProgramID,"Light",DefaultProgramUniformBlockLightID);
        mShaderProgramManager.createUniformBlock(DefaultProgramID,"Offsets",DefaultProgramUniformBlockOffsetID);

        mShaderProgramManager.bindUniformBlock(DefaultProgramUniformBlockViewID);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(Viewport),nullptr,GL_DYNAMIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(Viewport::view),sizeof(Viewport::projection),&mViewport.projection);

        mShaderProgramManager.bindUniformBlock(ParticleProgramUniformBlockViewportID);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(Viewport),nullptr,GL_DYNAMIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(Viewport::projection),sizeof(Viewport::view),&mViewport.projection);

        mShaderProgramManager.bindUniformBlock(ParticleProgramUniformBlockParticleID);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::vec4)*4096,nullptr,GL_DYNAMIC_DRAW);

        mShaderProgramManager.bindUniformBlock(DefaultProgramUniformBlockLightID);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(LightBlock),nullptr,GL_DYNAMIC_DRAW);

        mShaderProgramManager.bindUniformBlock(DefaultProgramUniformBlockOffsetID);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::vec4)*512,nullptr,GL_DYNAMIC_DRAW);

        mPlayer.mID = 12;
        mPlayer.mHealth = 4;
        mPlayer.mModelID = gPlayerModelID;
        mPlayer.mPosition = glm::vec3(0,0,0);
        mPlayer.mFireDelay = 100;

        mEnemyManager.mDespawnDistance = 60;
        mEnemyManager.mSpawnDistance = 60;

        mAudio.openAudio(48000,AUDIO_F32SYS,2,4096);
        mAudio.initAudioSubsytems(MIX_INIT_OGG);
        mAudio.loadAudioFiles("../Assets/sounds.txt");
        mAudio.allocateChannels(128);

        mProjectileManager.mXBounds = 60;
        mProjectileManager.mYBounds = 60;

        mParticleRenderer.mTexture.loadFromFile("../Assets/explosion_effect.png",0);
        mPlayerProjectileRenderer.init();
        mPlayerProjectileRenderer.mTexture.loadFromFile("../Assets/plasma.png",0);
        mEnemyProjectileRenderer.init();
        mEnemyProjectileRenderer.mTexture.loadFromFile("../Assets/enemy_blast.png",0);

        GUIButton button;
        button.mX = 0;
        button.mY = 100;
        button.mWidth = 200;
        button.mHeight = 100;
        mGUI.mButtons.push_back(button);

        Enemy enemy;
        enemy.mBoundingBox = boundingBox{glm::vec3(0,0,0),glm::vec3(2,2,2)};
        enemy.mFireRate = 2000;
        enemy.mHealth = 100;
        enemy.mHitFlashLength = 3503;
        enemy.mMilliSecStep = 0.01f;
        enemy.mModelID = 10;
        enemy.mType = Type::Enemy;

        for(unsigned int i = 0;i < 10;i++)
        {
            enemy.mID = 5*i;
            enemy.mPosition = glm::vec3(-30.0f + (5.0f*i),20,0);
            enemy.mMoveVel = glm::vec3(0.01,0,0);
            enemy.mNextFire = 50*i;
            mEnemyManager.mEnemies.push_back(enemy);
            enemy.mID = 55*i;
            enemy.mNextFire = 70*i;
            enemy.mPosition = glm::vec3(-30.0f + (5.0f*i),10,0);
            enemy.mMoveVel = glm::vec3(-.01,0,0);
            mEnemyManager.mEnemies.push_back(enemy);
        }

        mBackground.addVertexAttribute(GL_FLOAT,3);
        mBackground.addVertexAttribute(GL_FLOAT,3);

        mBackground.addVec3(-200,-200,5,0);
        mBackground.addVec3(200,-200,5,0);
        mBackground.addVec3(200,200,5,0);
        mBackground.addVec3(-200,200,5,0);
        mBackground.addVec3(0,0,-1,1);
        mBackground.addVec3(0,0,-1,1);
        mBackground.addVec3(0,0,-1,1);
        mBackground.addVec3(0,0,-1,1);

        mBackground.addIndex(0);
        mBackground.addIndex(1);
        mBackground.addIndex(2);

        mBackground.addIndex(0);
        mBackground.addIndex(2);
        mBackground.addIndex(3);
        mBackground.createVertexBuffers();
        mBackground.createIndexBuffers();

        mTextManager.init();
        SDL_Color col{255,255,255,255};
        mTextManager.addText("Score: 0",0.05,0.04,0.08,0.1,0.5,col);
        std::string health = "Health: " + std::to_string(mPlayer.mHealth);
        mTextManager.addText(health,0.85f,.04f,.08f,.1f,.5f,col);

        mState = GameState::Playing;
    }
}

void Game::handleEvents()
{

        mFireEventHandler.pushEvents(mPlayer.mFireEvents);
        mFireEventHandler.pushEvents(mEnemyManager.mFireEvents);

        mHitEventHandler.pushEvents(mEnemyManager.mHitEvents);
        mHitEventHandler.pushEvents(mPlayer.mHitEvents);

        mExplosionEventHandler.pushEvents(mEnemyManager.mExplosionEvents);

        while(mFireEventHandler.numEvents() > 0)
        {
            ProjectileFireEvent lastEvent = mFireEventHandler.getLastEvent();
            projectile proj;
            proj.bounds = lastEvent.bounds;
            proj.damage = lastEvent.damage;
            proj.modelID = lastEvent.modelID;
            proj.position = lastEvent.position;
            proj.type = lastEvent.type;
            proj.velocity = lastEvent.velocity;
            proj.hit = false;
            mProjectileManager.mProjectiles.push_back(proj);
            if(proj.type == Type::Player)
                mAudio.playChannel(lastEvent.soundEffectID,-1,0,0,0);
        }

        while(mExplosionEventHandler.numEvents() > 0)
        {
            ExplosionEvent lastEvent = mExplosionEventHandler.getLastEvent();
            ParticleSystem system;
            system.mEffectLife = lastEvent.effectLife;
            system.mEmitInterval = lastEvent.emitInterval;
            system.mEmitRate = lastEvent.emitRate;
            system.mLoop = false;
            system.mMaxSize = lastEvent.maxSize;
            system.mOrigin = lastEvent.origin;
            auto func = []() -> glm::vec3
            {
                return glm::sphericalRand(7.0f);
            };
            system.mRandomFunction = func;
            system.mStartLife = system.mEffectLife-1;
            system.create();
            mParticleRenderer.addParticleEffect(lastEvent.ID,system);
            mAudio.playChannel(lastEvent.explosionSoundEffectID,-1,0,0,0);
            mScore += 1;
            std::string score = "Score: " + std::to_string(mScore);
            SDL_Color col{255,255,255,255};
            mTextManager.setText(0,score,col);
        }

        while(mHitEventHandler.numEvents() > 0)
        {
            HitEvent lastEvent = mHitEventHandler.getLastEvent();
            mAudio.playChannel(lastEvent.hitEffectID,-1,0,0,0);
        }

}

void Game::update(uint32_t delta)
{
    if(mState == GameState::Playing)
    {
        mTotalTime += delta;

        if(mCursor.mPositionUpdated)
        {
            mCursor.mPositionUpdated = false;
            m2DElemArray.setElementRect(2,mCursor.mX,mCursor.mY,50,50);
        }
        mGUI.update(mCursor.mX,mCursor.mY);
        if(mGUI.mButtons[0].mActive)
        {
            m2DElemArray.setElementRect(0,0,100,210,110);
        }
        else
        {
            m2DElemArray.setElementRect(0,0,100,200,100);
        }

        handleEvents();

        if(mInput.keyHeld(LEFT))
        {
            mPlayer.mPosition = mPlayer.mPosition + glm::vec3(-0.091f *delta,0,0);
        }
        if(mInput.keyHeld(RIGHT))
        {
            mPlayer.mPosition = mPlayer.mPosition + glm::vec3(0.091f*delta,0,0);
        }
        if(mInput.keyHeld(UP))
        {
            mPlayer.mPosition = mPlayer.mPosition + glm::vec3(0,0.091f*delta,0);
        }
        if(mInput.keyHeld(DOWN))
        {
            mPlayer.mPosition = mPlayer.mPosition + glm::vec3(0,-.091f*delta,0);
        }
        if(mInput.keyHeld(FIRE))
        {
            mPlayer.fire();
        }
        glm::vec3& pos = mPlayer.mPosition;
        if(pos.x > 50)
        {
            pos.x = 50;
        }
        if(pos.x < -50)
        {
            pos.x = -50;
        }
        if(pos.y > mCamTarget.y + 25)
        {
            pos.y = mCamTarget.y + 25;
        }
        if(pos.y < mCamTarget.y - 25)
        {
            pos.y = mCamTarget.y - 25;
        }

        mPlayer.update(delta);
        SDL_Color txtCol{255,255,255,255};
        std::string health = "Health: " + std::to_string(mPlayer.mHealth);
        mTextManager.setText(1,health,txtCol);

        for(auto& enemy : mEnemyManager.mEnemies)
        {
            int damage = mProjectileManager.collide(enemy.mType,enemy.mBoundingBox);
            enemy.mHealth -= damage;
            if(damage > 0)
            {
                enemy.hit();
            }
        }

        int playerDamage = mProjectileManager.collide(mPlayer.mType,mPlayer.mBoundingBox);
        if(playerDamage > 0)
        {
            mPlayer.hit();
        }
        mPlayer.mHealth -= playerDamage;
        if(mPlayer.mHealth <= 0)
        {
            mGameRunning = false;
        }
        mProjectileManager.mScreenCenter = mCamTarget;
        mProjectileManager.update(delta);
        mEnemyManager.mScreenCenter = mCamTarget;
        mEnemyManager.mPlayerLocation = mPlayer.mPosition;
        mEnemyManager.update(delta);

        mMainCamera.followTarget(mCamTarget);
        mMainCamera.recreateViewMatrix();

        mParticleRenderer.update(delta);

        mLightManager.mLights.position[0] = glm::vec4(mCamTarget + glm::vec3(30,30,-20),170);

        mLightManager.getLights(mProjectileManager.mLights);
        mLightManager.getLights(mParticleRenderer.mLights);
    }
}

void Game::renderInterface()
{
    mWindow.clearViewport(0.1,0.1,0.1,1.0f);
    if(mState == GameState::Menu)
    {
        mShaderProgramManager.useProgram(TextProgramID);
        glProgramUniform1i(mShaderProgramManager.mPrograms[TextProgramID],mShaderProgramManager.mUniforms[TextProgramSamplerID],0);
        mShaderProgramManager.bindUniformBlock(TextProgramUniformBlockID);
        mMenuText.draw();

        mShaderProgramManager.useProgram(ImageArrayProgramID);
        glProgramUniform1i(mShaderProgramManager.mPrograms[ImageArrayProgramID],mShaderProgramManager.mUniforms[ImageArrayProgramSamplerID],0);
        mShaderProgramManager.bindUniformBlock(ImageArrayProgramUniformBlockID);
        mMenuImages.draw();

        mShaderProgramManager.useProgram(ImageProgramID);
        glProgramUniform1i(mShaderProgramManager.mPrograms[ImageProgramID],mShaderProgramManager.mUniforms[ImageProgramSamplerID],0);
        mShaderProgramManager.bindUniformBlock(ImageProgramUniformBlockID);
        mCursorImage.draw();

        mShaderProgramManager.useProgram(0);
    }
}

void Game::render()
{
    if(mState == GameState::Playing)
    {
        mViewport.view = mMainCamera.getViewMatrix();
        mShaderProgramManager.useProgram(DefaultProgramID);
        mShaderProgramManager.bindUniformBlock(DefaultProgramUniformBlockViewID);
        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(Viewport::view),&mViewport.view);
        mShaderProgramManager.bindUniformBlock(DefaultProgramUniformBlockLightID);
        mLightManager.uploadData();

        mShaderProgramManager.bindUniformBlock(DefaultProgramUniformBlockOffsetID);
        glm::vec4 pos = glm::vec4(mPlayer.mPosition,1);
        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::vec4),&pos);
        glm::vec4 colour = glm::vec4(mPlayer.mHitFlash,1,mPlayer.mHitFlash,1);
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::vec4)*256,sizeof(glm::vec4),&colour);
        mModelManager.draw(mPlayer.mModelID,GL_TRIANGLES,1);

        pos = glm::vec4(0,0,0,1);
        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::vec4),&pos);
        colour = glm::vec4(0,0,0,1);
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::vec4)*256,sizeof(glm::vec4),&colour);
        mBackground.draw(GL_TRIANGLES,1);

        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::vec4)*256,mEnemyManager.mOffsets.data());
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::vec4)*256,sizeof(glm::vec4)*256,mEnemyManager.mHitFlashes.data());
        mModelManager.draw(mEnemyManager.mEnemies.begin()->mModelID,GL_TRIANGLES,mEnemyManager.mEnemies.size());

        mShaderProgramManager.useProgram(ParticleProgramID);
        mShaderProgramManager.bindUniformBlock(ParticleProgramUniformBlockViewportID);
        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(Viewport::view),&mViewport.view);

        mShaderProgramManager.bindUniformBlock(ParticleProgramUniformBlockParticleID);
        mParticleRenderer.draw();


        mShaderProgramManager.bindUniformBlock(ParticleProgramUniformBlockViewportID);
        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(Viewport::view),&mViewport.view);
        glProgramUniform1i(mShaderProgramManager.mPrograms[ParticleProgramID],mShaderProgramManager.mUniforms[ParticleProgramSamplerID],0);
        mShaderProgramManager.bindUniformBlock(ParticleProgramUniformBlockParticleID);
        if(mProjectileManager.mPlayerOffsets)
        {
            glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::vec4)*mProjectileManager.mPlayerOffsets,mProjectileManager.mPlayerProjectileOffsets.data());
            mPlayerProjectileRenderer.draw(mProjectileManager.mPlayerOffsets);
        }
        if(mProjectileManager.mEnemyOffsets)
        {
            glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::vec4)*mProjectileManager.mEnemyOffsets,mProjectileManager.mEnemyProjectileOffsets.data());
            mEnemyProjectileRenderer.draw(mProjectileManager.mEnemyOffsets );
        }

        mShaderProgramManager.useProgram(TextProgramID);
        glProgramUniform1i(mShaderProgramManager.mPrograms[TextProgramID],mShaderProgramManager.mUniforms[TextProgramSamplerID],0);
        mShaderProgramManager.bindUniformBlock(TextProgramUniformBlockID);
        mTextManager.draw();

        mShaderProgramManager.useProgram(ImageArrayProgramID);
        glProgramUniform1i(mShaderProgramManager.mPrograms[ImageArrayProgramID],mShaderProgramManager.mUniforms[ImageArrayProgramSamplerID],0);
        mShaderProgramManager.bindUniformBlock(ImageArrayProgramUniformBlockID);
        m2DElemArray.draw();
    }

    mShaderProgramManager.useProgram(0);
    mWindow.swapWindow();
}

void Game::close()
{
    mGameRunning = false;
}
