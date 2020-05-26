#include "Game.h"
Game::Game() : mWindow(GLWindow()), mShaderProgramManager(GLProgramManager()),  mMainCamera(Camera()),
 mInput(InputHandler()), mModelManager(GLModelManager()),
mTotalTime(0),  mCamTarget(0,0,0), mPlayer(Player()),
mProjectileManager(ProjectileManager()),
mFireEventHandler(),mHitEventHandler(),mExplosionEventHandler(),   mEnemyManager(EnemyManager()),
mParticleRenderer(ParticleRenderer()),   mLightManager(LightManager()),
m2DElemArray(GL2DElementArray()),mMenuImages(GL2DElementArray()),
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
        mState = GameState::GameLoading;    }
}

void Game::updateInterface(const uint32_t& delta)
{
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
    }
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

        mProjectileManager.mXBounds = 60;
        mProjectileManager.mYBounds = 60;

        mParticleRenderer.mTexture.loadFromFile("../Assets/explosion_effect.png",0);
        mPlayerProjectileRenderer.init();
        mPlayerProjectileRenderer.mTexture.loadFromFile("../Assets/plasma.png",0);
        mEnemyProjectileRenderer.init();
        mEnemyProjectileRenderer.mTexture.loadFromFile("../Assets/enemy_blast.png",0);

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
            mScore += 1;
            std::string score = "Score: " + std::to_string(mScore);
            SDL_Color col{255,255,255,255};
        }


}

void Game::update(uint32_t delta)
{
    if(mState == GameState::Playing)
    {
        mTotalTime += delta;

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

void Game::render()
{
    if(mState == GameState::Playing)
    {
      mWindow.clearViewport(0.1,0.1,0.1,1.0f);
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
