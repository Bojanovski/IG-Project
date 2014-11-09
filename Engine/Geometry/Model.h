#ifndef EN_MODEL_H
#define EN_MODEL_H

#include <Engine/Geometry/TriangleMesh.h>
#include <Engine/Geometry/Material.h>

namespace engine
{
    class Model
    {
    public:

        std::vector<TriangleMesh*> meshes;
        std::vector<Material*> materials;
    };
}

#endif
