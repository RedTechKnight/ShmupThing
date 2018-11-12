#include "GLCamera.h"

Camera::Camera() : mViewMat(glm::mat4()),mProjectionMat(glm::mat4()),mUpright(1),
mRightVec(1,0,0),mUpVec(0,1,0),mLookAtVec(0,0,1),mEyePos(0,0,0),mTarget(0,0,0),mOrientationUpdated(false),mFollowTarget(false)
{

}

Camera::~Camera()
{
    mViewMat = glm::mat4();
    mProjectionMat = glm::mat4();
}

void Camera::createViewMatrix(const glm::vec3& eyePos,const glm::vec3& viewDir)
{
    mEyePos = eyePos;
    mLookAtVec = glm::normalize(viewDir);

    mViewMat[0][0] = mRightVec.x;
    mViewMat[1][0] = mRightVec.y;
    mViewMat[2][0] = mRightVec.z;
    mViewMat[3][0] = -glm::dot(mEyePos,mRightVec);

    mViewMat[0][1] = mUpVec.x;
    mViewMat[1][1] = mUpVec.y;
    mViewMat[2][1] = mUpVec.z;
    mViewMat[3][1] = -glm::dot(mEyePos,mUpVec);

    mViewMat[0][2] = mLookAtVec.x;
    mViewMat[1][2] = mLookAtVec.y;
    mViewMat[2][2] = mLookAtVec.z;
    mViewMat[3][2] = -glm::dot(mEyePos,mRightVec);

    mViewMat[0][3] = 0;
    mViewMat[1][3] = 0;
    mViewMat[2][3] = 0;
    mViewMat[3][3] = 1;
}

void Camera::followTarget(const glm::vec3& target)
{
    if(!mFollowTarget)
    {
        mFollowTarget = true;
    }
    mTarget = target;

}

void Camera::recreateViewMatrix()
{
    if(mOrientationUpdated)
    {
        if(!mFollowTarget)
        {
            mLookAtVec = mUpright * glm::vec3(0,0,1);
            mRightVec = mUpright * glm::vec3(1,0,0);
            mUpVec = mUpright * glm::vec3(0,1,0);
        }
        mOrientationUpdated = false;
    }

    mViewMat[0][0] = mRightVec.x;
    mViewMat[1][0] = mRightVec.y;
    mViewMat[2][0] = mRightVec.z;
    mViewMat[3][0] = -glm::dot(mEyePos+mTarget,mRightVec);

    mViewMat[0][1] = mUpVec.x;
    mViewMat[1][1] = mUpVec.y;
    mViewMat[2][1] = mUpVec.z;
    mViewMat[3][1] = -glm::dot(mEyePos+mTarget,mUpVec);

    mViewMat[0][2] = mLookAtVec.x;
    mViewMat[1][2] = mLookAtVec.y;
    mViewMat[2][2] = mLookAtVec.z;
    mViewMat[3][2] = -glm::dot(mEyePos+mTarget,mLookAtVec);

    mViewMat[0][3] = 0;
    mViewMat[1][3] = 0;
    mViewMat[2][3] = 0;
    mViewMat[3][3] = 1;
}

void Camera::createProjectionMatrix(const float& nearPlane,const float& farPlane,const float& FOV,const float& aspectRatio)
{
    mProjectionMat = glm::mat4(1.0f/(aspectRatio * glm::tan(FOV/2)),  0,0,0,
                               0,   1.0f/glm::tan(FOV/2),   0,0,
                               0,0, farPlane/(farPlane-nearPlane),    1,
                               0,0, -nearPlane * farPlane /(farPlane- nearPlane),   0);
}

void Camera::pitch(const float& angle)
{

    glm::mat3 rot;
    rot = glm::rotate(glm::mat4(1),-angle,mRightVec);
    mUpright = mUpright * rot;
    mEyePos = rot * mEyePos;
    mLookAtVec = rot * mLookAtVec;
    mRightVec = rot * mRightVec;
    mUpVec = rot * mUpVec;
    mOrientationUpdated = true;
}

void Camera::heading(const float& angle)
{
    glm::mat3 rot;
    rot = glm::rotate(glm::mat4(1),angle,glm::vec3(0,1,0));
    mUpright = mUpright * rot;
    mEyePos = rot * mEyePos;
    mLookAtVec = rot * mLookAtVec;
    mRightVec = rot * mRightVec;
    mUpVec = rot * mUpVec;
    mOrientationUpdated = true;
}

void Camera::move(const glm::vec3& dir)
{
    mEyePos = mEyePos + (mUpright * dir);
}

glm::mat3 Camera::getUprightMatrix()
{
    return mUpright;
}

glm::mat4 Camera::getViewMatrix()
{
    return mViewMat;
}

glm::mat4 Camera::getProjectionMatrix()
{
    return mProjectionMat;
}
