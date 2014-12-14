#ifndef EN_INSTANCEDMODEL_H
#define EN_INSTANCEDMODEL_H

#include <Engine/Geometry/Model.h>

namespace engine
{
    class InstancedModel : public Model
    {
    public:
        void LoadToGPU();
        void CleanUp();

        std::vector<glm::mat4> transforms;
        GLuint transforms_VBO;
    };
}

#endif
