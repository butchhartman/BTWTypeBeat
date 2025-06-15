#ifndef _ENGINE_CAMERA_H_
#define _ENGINE_CAMERA_H_

#include <gml.hpp>

namespace EngineClasses {

class Camera {
    private:
        float cameraSpeed;
        float sensitivity;

        float yaw;
        float pitch;

        float mouseLockX;
        float mouseLockY;

        bool forward;
        bool backward;
        bool right; 
        bool left;

        gml::Vec3 CameraPos;
        gml::Vec3 CameraFront;
        gml::Vec3 CameraUp;

    public:
        Camera(
            gml::Vec3 CameraPos,
            float cameraSpeed,
            float sensitivity,
            float mouseLockX,
            float mouseLockY
        );
        ~Camera();

        void setMouseLockPosition(float mouseLockX, float mouseLockY);

        void setForward(bool status);
        void setBackward(bool status);
        void setLeft(bool status);
        void setRight(bool status);

        void updatePosition(double deltaTime);
        void updateRotation(float newMouseXPos, float newMouseYPos);
        const float* getViewMat();
};

} // namespace EngineClasses

#endif // _ENGINE_CAMERA_H_