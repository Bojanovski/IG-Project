#include <Engine/Geometry/Model.h>
#include <Engine/Common/ErrorCheck.h>

namespace engine
{
    GLsizei Model::VertexDataSizeInBytes() const
    {
        GLsizei ret = 0;
        for(const auto &mesh : meshes)
            ret += mesh.TotalSizeInBytes();
        return ret;
    }

    GLsizei Model::IndexDataSizeInBytes() const
    {
        GLsizei ret = 0;
        for(const auto &mesh : meshes)
            ret += mesh.indices.GetSizeInBytes();
        return ret;
    }

    void Model::LoadToGPU()
    {
        for(auto &mesh : meshes)
        {
            mesh.LoadToGPU();
            GLErrCheck(false);
        }
    }
}
