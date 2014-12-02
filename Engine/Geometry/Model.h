#ifndef EN_MODEL_H
#define EN_MODEL_H

#include <Engine/Geometry/TriangleMesh.h>
#include <Engine/Geometry/Material.h>

namespace engine
{
    class Model
    {
    public:
        GLsizei VertexDataSizeInBytes() const;
        GLsizei IndexDataSizeInBytes() const;

        void LoadToGPU();

        std::vector<TriangleMesh> meshes;
        std::vector<Material> materials;
    };
}

#endif
