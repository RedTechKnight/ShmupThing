#ifndef _GL_CAMERA_H
#define _GL_CAMERA_H
#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <ext/scalar_constants.hpp>

class Camera
{
    private:
        glm::mat4 mViewMat;
        glm::mat4 mProjectionMat;
        glm::mat3 mUpright;
        glm::vec3 mRightVec;
        glm::vec3 mUpVec;
        glm::vec3 mLookAtVec;
        glm::vec3 mEyePos;
        glm::vec3 mTarget;
        bool mOrientationUpdated;
        bool mFollowTarget;
    public:
        Camera();
        ~Camera();

        void createViewMatrix(const glm::vec3& eyePos,const glm::vec3& viewDir);
        void followTarget(const glm::vec3& target);
        void recreateViewMatrix();
        void createProjectionMatrix(const float& nearPlane,const float& farPlane,const float& FOV,const float& aspectRatio);
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();
        glm::mat3 getUprightMatrix();

        void heading(const float& angle);
        void pitch(const float& angle);
        void move(const glm::vec3& dir);
};


#endif // _GL_CAMERA_H
