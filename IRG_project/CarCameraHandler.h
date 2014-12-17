/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef ACG_CARCAMERAHANDLER_H
#define ACG_CARCAMERAHANDLER_H

#include <Engine/Core/CameraHandler.h>
#include <Engine/Physics/ChassisCarController.h>

class CarCameraHandler :
    public engine::CameraHandler
{
public:
    CarCameraHandler(const engine::Camera &cam, const engine_physics::ChassisCarController *car);
    ~CarCameraHandler(void);

    void HandleEvent(const SDL_Event &e);

    void KeyPress(const SDL_KeyboardEvent &e);
    void MouseMotion(const SDL_MouseMotionEvent &e);
    void WindowResize(const SDL_WindowEvent &e);

    void Rotate(float yaw, float pitch);
    void Update(float dt);

    const glm::mat4& GetProjectionMatrix() const;
    const glm::mat4& GetViewMatrix() const;

    float speed;
    float rotationSpeed;

private:
    const engine_physics::ChassisCarController *car;

    glm::vec3 dir;
    float springiness;
    float dx, dy;
};

#endif
