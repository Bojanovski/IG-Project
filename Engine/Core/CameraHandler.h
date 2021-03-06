/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_CAMERAHANDLER_H
#define EN_CAMERAHANDLER_H

#include <Engine/Core/EventListener.h>
#include <Engine/Core/Camera.h>
#include <Engine/Core/Updateable.h>

namespace engine
{
    class CameraHandler :
        public EventListener, public Updateable
    {
    public:
        CameraHandler(const Camera &cam);
        virtual ~CameraHandler(void);

        void SetAspectRatio(float asRatio);

        virtual const glm::mat4& GetProjectionMatrix() const = 0;
        virtual const glm::mat4& GetViewMatrix() const = 0;
        const glm::vec3& GetPosition() const;

        Camera cam;
    protected:
        float far, near;
    };
}

#endif
