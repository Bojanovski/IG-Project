#include "Car.h"
#include <Engine/Geometry/ObjectLoader.h>

using namespace engine;
using namespace std;

void CarModel::LoadModel(const string &path)
{
    static const string files[] = {
        "carbody.obj",
        "leftfronttire.obj",
        "rightfronttire.obj",
        "leftbacktire.obj",
        "rightbacktire.obj",
    };

    for(const auto &file : files)
    {
        car.materials.push_back(Material());
        car.meshes.push_back(TriangleMesh());

        Material &mat = car.materials[car.materials.size() - 1];
        LoadObj(path, file, mat, car.meshes[car.meshes.size() - 1]);
        mat.diffuse_tex.GenerateMipmaps();
        mat.diffuse_tex.TexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        mat.diffuse_tex.TexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        mat.diffuse_tex.TexParami(GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
        mat.diffuse_tex.TexParami(GL_TEXTURE_WRAP_T, GL_REPEAT);
        mat.diffuse_tex.TexParami(GL_TEXTURE_WRAP_S, GL_REPEAT);
    }

    car.LoadToGPU();
}

glm::mat4& CarModel::GetPartTransform(CarPart part)
{
    return car.meshes[part].transform;
}

const glm::mat4& CarModel::GetPartTransform(CarPart part) const
{
    return car.meshes[part].transform;
}

const engine::Model& CarModel::GetModel() const
{
    return car;
}
