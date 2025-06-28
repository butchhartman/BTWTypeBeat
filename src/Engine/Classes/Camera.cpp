#include <Engine/Classes/Camera.hpp>
#include <cmath>
#define RAD(angleMeasure) (float)(angleMeasure * (3.14159265358979323846 / 180.0))

namespace EngineClasses {

Camera::Camera(
    gml::Vec3 CameraPos, 
    float cameraSpeed, 
    float sensitivity,
    float mouseLockX, 
    float mouseLockY,
    float fov,
    float aspectRatio,
    float viewDistance
    )   
    // Dumb syntax needed so that the vec3 members wont try to initialize with
    // an invalid constructor. Fix this by adding a no parameter default
    // constructor to vec3 which intializes everything to 0
    : CameraPos(0.0f, 0.0f, 0.0f), 
    CameraFront(0.0f, 0.0f, -1.0f), 
    CameraUp(0.0f, 1.0f, 0.0f) 
    {

    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->nearPlane = 0.1f; // constant near plane
    this->farPlane = viewDistance;

    this->CameraPos = CameraPos;
    this->CameraFront = gml::Vec3(0.0f, 0.0f, -1.0f);
    this->CameraUp = gml::Vec3(0.0f, 1.0f, 0.0f);

    this->yaw = -90.0f;
    this->pitch = 0.0f;

    this->mouseLockX = mouseLockX;
    this->mouseLockY = mouseLockY;

    this->forward = false;
    this->backward = false;
    this->left = false;
    this->right = false;

    this->cameraSpeed = cameraSpeed;
    this->sensitivity = sensitivity;

}

Camera::~Camera() {

}

void Camera::setForward(bool status) {
    this->forward = status;
}

void Camera::setBackward(bool status) {
    this->backward = status;
}

void Camera::setLeft(bool status) {
    this->left = status;
}

void Camera::setRight(bool status) {
    this->right = status;
}
void Camera::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
}

void Camera::setMouseLockPosition(float mouseLockX, float mouseLockY) {
    this->mouseLockX = mouseLockX;
    this->mouseLockY = mouseLockY;
}

void Camera::updatePosition(double deltaTime) {
    this->CameraFront = gml::Vec3(
        std::cosf(RAD(this->yaw)) * std::cosf(RAD(this->pitch)),
        std::sinf(RAD(this->pitch)),
        std::sinf(RAD(this->yaw)) * std::cosf(RAD(this->pitch))
    ).normalize();

    if (forward) {
        this->CameraPos = 
            this->CameraPos + 
            this->CameraFront * 
            this->cameraSpeed * 
            (float)deltaTime;
    }
    if (left) {
        this->CameraPos = 
            this->CameraPos - 
            (this->CameraFront.cross(this->CameraUp)).normalize() * 
            cameraSpeed * 
            (float)deltaTime;
    }
    if (backward) {
        this->CameraPos = 
            this->CameraPos - 
            this->CameraFront *
             this->cameraSpeed * 
             (float)deltaTime;
    }
    if (right) {
        this->CameraPos = 
            this->CameraPos + 
            (this->CameraFront.cross(this->CameraUp)).normalize() * 
            this->cameraSpeed * (float)deltaTime;
    }

}

void Camera::updateRotation(float newMouseXPos, float newMouseYPos) {
    float xOffset = newMouseXPos - this->mouseLockX; 
    float yOffset = this->mouseLockY - newMouseYPos;
    
    // lastX = (float)event.xPos;
    // lastY = (float)event.yPos;

    xOffset *= sensitivity;
    yOffset *= sensitivity;
    
    // Must constrain yaw vaules to 0-360 or else floating point precision
    // will be lost with high yaw values. Thanks to Lonami's comment on
    // https://learnopengl.com/Getting-started/Camera
    this->yaw = std::fmod(this->yaw + xOffset, 360.0f);
    this->pitch += yOffset;

    if (this->pitch > 89.9f) {
        this->pitch = 89.9f;
    } else if (this->pitch < -89.9f) {
        this->pitch = -89.9f;
    }

}

gml::Mat4 Camera::getViewMat() {
    return gml::Mat4::lookAt(
        this->CameraPos,
        this->CameraPos + this->CameraFront,
        this->CameraUp);
}

gml::Mat4 Camera::getPerspectiveMat() {
    return gml::Mat4::perspectiveProjection(
        this->fov,
        this->aspectRatio,
        this->nearPlane,
        this->farPlane
    );
}

gml::Vec3 Camera::getCameraPosition() {
    return this->CameraPos;
}

} // namespace EngineClasses