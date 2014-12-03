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
        LoadObj(path, file, car.materials[car.materials.size() - 1], car.meshes[car.meshes.size() - 1]);
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
