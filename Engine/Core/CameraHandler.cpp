/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/CameraHandler.h>

namespace engine
{
    using namespace glm;

    CameraHandler::CameraHandler(const Camera &cam)
        : cam(cam), near(0.1f), far(1000.0f)
    {
    }

    CameraHandler::~CameraHandler(void)
    {
    }

    const vec3& CameraHandler::GetPosition() const
    {
        return cam.position;
    }

    void CameraHandler::SetAspectRatio(float asRatio)
    {
        cam.aspectRatio = asRatio;
        cam.ComputeProjection(near, far);
    }

}
