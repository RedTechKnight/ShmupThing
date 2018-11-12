#include "Game.h"
Game::Game() : mOpenGL(DragonGL()), mShaderProgramManager(GLProgramManager()),  mMainCamera(Camera()),
 mInput(InputHandler()), mModelManager(GLModelManager()),
mTotalTime(0),  mCamTarget(0,0,0),  mLevel(Level()), mPlayer(Player()),
mProjectileManager(ProjectileManager()),
mFireEventHandler(),mHitEventHandler(),mExplosionEventHandler(),
mAudio(MixerAudio()),   mEnemyManager(EnemyManager()), mResourceLoader(ResourceLoader()),
mParticleRenderer(ParticleRenderer()),   mLightManager(LightManager()),
mCursorImage(GL2DElement()), mCursor(Cursor()),
m2DElemArray(GL2DElementArray()),mGUI(GUISystem()),mTextManager(GLTextManager()),
mMenu(GUISystem()),mMenuText(GLTextManager()),mMenuImages(GL2DElementArray()),
mCurrentRes(0),mPlayerProjectileRenderer(GLProjectileRenderer()),
mEnemyProjectileRenderer(GLProjectileRenderer()),mBackground(GLModel()),
mGameRunning(true),mState(GameState::Initialising),mMenuState(MenuState::LoadingMain)
{

}

Game::~Game()
{
    mGameRunning = false;
}

const unsigned int LEFT = 1;
const unsigned int RIGHT = 2;
const unsigned int UP = 4;
const unsigned int DOWN = 8;
const unsigned int FIRE = 16;
const unsigned int QUIT = 32;

void Game::start()
{
    if(mState == GameState::Initialising)
    {
        mOpenGL.initSDL("SHMUP",1280,720,4,5);
        glEnable(GL_TEXTURE_3D);
        mShaderProgramManager.createProgram(TextProgramID);
        mShaderProgramManager.loadShader("textVertex.glsl",GL_VERTEX_SHADER,TextProgramID);
        mShaderProgramManager.loadShader("textGeometry.glsl",GL_GEOMETRY_SHADER,TextProgramID);
        mShaderProgramManager.loadShader("textFragment.glsl",GL_FRAGMENT_SHADER,TextProgramID);
        mShaderProgramManager.linkProgram(TextProgramID);

        mShaderProgramManager.createProgram(ImageProgramID);
        mShaderProgramManager.loadShader("2delemvertex.glsl",GL_VERTEX_SHADER,ImageProgramID);
        mShaderProgramManager.loadShader("2delemgeometry.glsl",GL_GEOMETRY_SHADER,ImageProgramID);
        mShaderProgramManager.loadShader("2delemfragment.glsl",GL_FRAGMENT_SHADER,ImageProgramID);
        mShaderProgramManager.linkProgram(ImageProgramID);

        mShaderProgramManager.createProgram(ImageArrayProgramID);
        mShaderProgramManager.loadShader("2delemarrayvertex.glsl",GL_VERTEX_SHADER,ImageArrayProgramID);
        mShaderProgramManager.loadShader("2delemarraygeometry.glsl",GL_GEOMETRY_SHADER,ImageArrayProgramID);
        mShaderProgramManager.loadShader("2delemarrayfragment.glsl",GL_FRAGMENT_SHADER,ImageArrayProgramID);
        mShaderProgramManager.linkProgram(ImageArrayProgramID);

        m2DElemArray.init();

        mMenuImages.init();

        SDL_Color textColor = {255,0,0,255};

        mTextManager.setFont("data-latin.ttf",20);

        textColor = {255,255,255,255};
        mMenuText.setFont("data-latin.ttf",20);
        mMenuText.init();

        mCursorImage.init();
        mCursorImage.mTexture.loadFromFile("cursor.png",0);
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
            mMenu.reset();
            mMenuText.reset();
            mMenuImages.reset();
            GUIButton button;
            button.mElementID = 0;
            button.mHeight = .1;
            button.mWidth = .2;
            button.mX = .4;
            button.mY = .2;
            button.mType = GUIElemType::Text;
            mMenu.addElement(button.mElementID,std::make_unique<GUIButton>(button));

            button.mElementID = 1;
            button.mY = .3;
            button.mWidth = .16;
            mMenu.addElement(button.mElementID,std::make_unique<GUIButton>(button));

            button.mElementID = 2;
            button.mY = .4;
            button.mWidth = .10;
            mMenu.addElement(button.mElementID,std::make_unique<GUIButton>(button));

            button.mElementID = 3;
            button.mY = .5f;
            button.mWidth = .08;
            mMenu.addElement(button.mElementID,std::make_unique<GUIButton>(button));
            SDL_Color col = {255,255,255,255};
            mMenuText.addText("Start Game",0,0,0,0,.1,col);
            mMenuText.addText("Settings",0,0,0,0,.1,col);
            mMenuText.addText("About",0,0,0,0,.1,col);
            mMenuText.addText("Quit",0,0,0,0,.1,col);
            for(auto& elem: mMenu.mElements)
            {
                if(elem.second->mType == GUIElemType::Text)
                {

                    mMenuText.setTextPosition(elem.second->mElementID,elem.second->mX,elem.second->mY,
                                          elem.second->mWidth,elem.second->mHeight,.1f);
                }
                else if(elem.second->mType == GUIElemType::Image)
                {
                    mMenuImages.setElementRect(elem.second->mElementID,elem.second->mX,elem.second->mY,
                                               elem.second->mWidth,elem.second->mHeight);
                    mMenuImages.setTextureArea(elem.second->mElementID,0,0,1,1);
                    mMenuImages.setElementDepth(elem.second->mElementID,.1f);
                }
            }
            mMenuState = MenuState::Main;
        }


        if(mMenuState == MenuState::Main)
        {
            mMenu.update(mCursor.mX,mCursor.mY);
            for(auto& elem: mMenu.mElements)
            {
                if(elem.second->mType == GUIElemType::Text)
                {
                    mMenuText.setColour(elem.second->mElementID,1,1,1);
                    if(elem.second->mActive)
                    {
                        mMenuText.setColour(elem.second->mElementID,1,0,0);
                    }
                    if(elem.second->mClicked)
                    {
                        mMenuText.setColour(elem.second->mElementID,0,1,0);
                    }
                }
                else if(elem.second->mType == GUIElemType::Image)
                {
                    if(elem.second->mActive)
                    {
                        mMenuImages.setElementRect(elem.second->mElementID,elem.second->mX,elem.second->mY,elem.second->mWidth+10,
                                                   elem.second->mHeight+10);
                    }
                    if(elem.second->mClicked)
                    {
                        mMenuImages.setElementRect(elem.second->mElementID,elem.second->mX,elem.second->mY,elem.second->mWidth-5,
                                                   elem.second->mHeight-5);
                    }
                }

                if(elem.second->mClicked)
                {
                    if(elem.second->mElementID == 0)
                    {
                        mState = GameState::GameLoading;
                    }
                    if(elem.second->mElementID == 1)
                    {
                        mMenuState = MenuState::LoadingSettings;
                    }
                    if(elem.second->mElementID == 3)
                    {
                        mGameRunning = false;
                    }
                }
            }
        }
        if(mMenuState == MenuState::LoadingSettings)
        {
            mMenu.reset();
            mMenuText.reset();
            mMenuImages.reset();
            SDL_Color col = {255,255,255,255};
            mMenuText.addText("Resolution: ",.1f,.1f,.2f,.05f,.1f,col);
            mMenuText.addText("Windowed: ",.1f,.2f,.2,.05f,.1f,col);
            mMenuText.addText("Quality: ",.1f,.3f,.2,.05f,.1f,col);
            mMenuText.addText("Back: ",0,0,0,0,.4f,col);
            mMenuText.addText("1280 x 720",.6,.1,.1,.05,.1,col);
            mMenuText.addText("Apply",0,0,0,0,.1,col);

            mMenuImages.addElement("left_arrow.png");
            mMenuImages.addElement("right_arrow.png");
            mMenuImages.addElement("checkbox.png");

            GUIButton button;
            button.mElementID = 3;
            button.mHeight = .05f;
            button.mWidth = .1f;
            button.mX = .1f;
            button.mY = .5f;
            button.mType = GUIElemType::Text;
            mMenu.addElement(button.mElementID,std::make_unique<GUIButton>(button));
            button.mHeight = .05;
            button.mWidth = .1f;
            button.mX = .8f;
            button.mY = .1f;
            button.mElementID = 5;
            mMenu.addElement(button.mElementID,std::make_unique<GUIButton>(button));

            button.mElementID = 0;
            button.mHeight = .05f;
            button.mWidth = .05f;
            button.mX = .5;
            button.mY = .1;
            button.mType = GUIElemType::Image;
            mMenu.addElement(button.mElementID,std::make_unique<GUIButton>(button));

            button.mElementID = 1;
            button.mX = .71;
            mMenu.addElement(button.mElementID,std::make_unique<GUIButton>(button));

            GUICheckBox checkbox;
            checkbox.mElementID = 2;
            checkbox.mHeight = .05;
            checkbox.mWidth = .05;
            checkbox.mType = GUIElemType::Image;
            checkbox.mX = .5;
            checkbox.mY = .2;
            mMenu.addElement(checkbox.mElementID,std::make_unique<GUICheckBox>(checkbox));

            for(auto& elem: mMenu.mElements)
            {
                if(elem.second->mType == GUIElemType::Text)
                {
                    mMenuText.setTextPosition(elem.second->mElementID,elem.second->mX,elem.second->mY,
                                          elem.second->mWidth,elem.second->mHeight,.1f);
                }

                if(elem.second->mType == GUIElemType::Image)
                {
                    mMenuImages.setElementRect(elem.second->mElementID,elem.second->mX,elem.second->mY,
                                               elem.second->mWidth,elem.second->mHeight);
                    mMenuImages.setElementDepth(elem.second->mElementID,.1f);
                    mMenuImages.setTextureArea(elem.second->mElementID,0,.5f,1,.5f);
                }
            }
            mMenuState = MenuState::Settings;
        }

        if(mMenuState == MenuState::Settings)
        {
            SDL_DisplayMode mode = mOpenGL.mDisplayModes[0][mCurrentRes];
            std::string resolution = "";
            resolution.append(std::to_string(mode.w));
            resolution.append(" x ");
            resolution.append(std::to_string(mode.h));
            SDL_Color col = {255,255,255,255};
            mMenuText.setText(4,resolution,col);
            mMenu.update(mCursor.mX,mCursor.mY);
            for(auto& elem: mMenu.mElements)
            {
                if(elem.second->mType == GUIElemType::Text)
                {
                    mMenuText.setColour(elem.second->mElementID,1,1,1);
                    if(elem.second->mActive)
                    {
                        mMenuText.setColour(elem.second->mElementID,1,0,0);
                    }
                    if(elem.second->mClicked)
                    {
                        mMenuText.setColour(elem.second->mElementID,0,1,0);
                    }
                }

                else if(elem.second->mType == GUIElemType::Image)
                {
                    mMenuImages.setTextureArea(elem.second->mElementID,0,.5,1,.5);
                    if(elem.second->mActive)
                    {
                        mMenuImages.setTextureArea(elem.second->mElementID,0,0,1,.5);
                    }
                    if(elem.second->mClicked)
                    {

                    }
                }

                if(elem.second->mClicked)
                {
                    if(elem.second->mElementID == 3)
                    {
                        mMenuState = MenuState::LoadingMain;
                    }
                    if(elem.second->mElementID == 0)
                    {
                        if(mCurrentRes > 0)
                            mCurrentRes--;
                    }
                    if(elem.second->mElementID == 1)
                    {
                        if(mCurrentRes < mOpenGL.mDisplayModes[0].size()-1)
                            mCurrentRes++;
                    }
                    if(elem.second->mElementID == 5)
                    {

                    }
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

        mModelManager.batchLoad("models.txt");

        mLevel.loadLevel("level01.txt");
        mShaderProgramManager.createProgram(DefaultProgramID);
        mShaderProgramManager.loadShader("defaultVertex.glsl",GL_VERTEX_SHADER,DefaultProgramID);
        mShaderProgramManager.loadShader("defaultFragment.glsl",GL_FRAGMENT_SHADER,DefaultProgramID);
        mShaderProgramManager.linkProgram(DefaultProgramID);

        mShaderProgramManager.createProgram(ParticleProgramID);
        mShaderProgramManager.loadShader("particlesVertex.glsl",GL_VERTEX_SHADER,ParticleProgramID);
        mShaderProgramManager.loadShader("particlesGeometry.glsl",GL_GEOMETRY_SHADER,ParticleProgramID);
        mShaderProgramManager.loadShader("particlesFragment.glsl",GL_FRAGMENT_SHADER,ParticleProgramID);
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
        mAudio.loadAudioFiles("sounds.txt");
        mAudio.allocateChannels(128);

        mProjectileManager.mXBounds = 60;
        mProjectileManager.mYBounds = 60;

        mLightManager.mLights.position[0] = glm::vec4(0,0,0,30);
        mLightManager.mLights.colour[0] = glm::vec4(1,1,1,1);

        mParticleRenderer.mTexture.loadFromFile("explosion_effect.png",0);
        mPlayerProjectileRenderer.init();
        mPlayerProjectileRenderer.mTexture.loadFromFile("plasma.png",0);
        mEnemyProjectileRenderer.init();
        mEnemyProjectileRenderer.mTexture.loadFromFile("enemy_blast.png",0);

        GUIButton button;
        button.mX = 0;
        button.mY = 100;
        button.mWidth = 200;
        button.mHeight = 100;
        mGUI.addElement(10,std::make_unique<GUIButton>(button));

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

        mState = GameState::Playing;
    }
}

void Game::handleEvents()
{

        mFireEventHandler.pushEvents(mPlayer.mFireEvents);
        mFireEventHandler.pushEvents(mEnemyManager.mFireEvents);

        mHitEventHandler.pushEvents(mEnemyManager.mHitEvents);

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
        if(mGUI.mElements[10]->mActive)
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
        mPlayer.mHealth -= playerDamage;
        if(mPlayer.mHealth < 0)
        {
            mGameRunning = false;
        }
        mProjectileManager.mScreenCenter = mCamTarget;
        mProjectileManager.update(delta);
        mEnemyManager.mScreenCenter = mCamTarget;
        mEnemyManager.mPlayerLocation = mPlayer.mPosition;
        mEnemyManager.update(delta);

        mMainCamera.followTarget(mCamTarget);
        //mCamTarget.y += 0.01f*delta;
        //mPlayer.mPosition.y += 0.01f*delta;
        mMainCamera.recreateViewMatrix();

        mParticleRenderer.update(delta);

        mLightManager.mLights.position[0] = glm::vec4(mCamTarget + glm::vec3(30,30,-20),170);

        mLightManager.getLights(mProjectileManager.mLights);
        mLightManager.getLights(mParticleRenderer.mLights);
    }
}

void Game::renderInterface()
{
    mOpenGL.clearViewport(0.1,0.1,0.1,1.0f);
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
        glm::vec4 colour = glm::vec4(0,1,0,1);
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
    mOpenGL.swapWindow();
}

void Game::close()
{
    mGameRunning = false;
}
