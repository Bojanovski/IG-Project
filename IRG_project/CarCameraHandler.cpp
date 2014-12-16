#include "CarCameraHandler.h"
#include <Engine/Core/SDLHandler.h>
#include <Engine/Core/EventHandler.h>
#include <Engine/Common/MathFunctions.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace engine;
using namespace engine_physics;

CarCameraHandler::CarCameraHandler(const Camera &cam, const ChassisCarController *car) :
    CameraHandler(cam), speed(speed), rotationSpeed(rotationSpeed), springiness(100), dx(0.0f), dy(0.0f), near(0.1f), far(1000.0f),
    car(car)
{
    this->cam.phiy = -PI / 10.0f;
    this->cam.ComputeView();
    this->cam.ComputeProjection(near, far);
}

CarCameraHandler::~CarCameraHandler(void)
{
}

void CarCameraHandler::HandleEvent(const SDL_Event &e)
{
    switch(e.type)
    {
    case SDL_WINDOWEVENT:
        WindowResize(e.window);
        break;
    }
}

void CarCameraHandler::WindowResize(const SDL_WindowEvent &e)
{
    if(e.windowID == SDL_WINDOWEVENT_RESIZED)
    {
        cam.aspectRatio = static_cast<float>(e.data1) / static_cast<float>(e.data2);
        cam.ComputeProjection(near, far);
    }
}

void CarCameraHandler::Update(float deltaTime)
{
    static const vec4 offset(0.0f, 1.5f, -4.0f, 1.0f);
    static const vec3 Y(0.0f, 1.0f, 0.0f);
    cam.phix = car->GetYRot();
    cam.position = car->GetPos() + vec3(offset * rotate(I, -cam.phix * 180.0f / PI, Y));
    cam.ComputeView();
}

const mat4& CarCameraHandler::GetProjectionMatrix() const
{
    return cam.GetProjectionMatrix();
}

const mat4& CarCameraHandler::GetViewMatrix() const
{
    return cam.GetViewMatrix();
}
