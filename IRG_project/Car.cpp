#include "Car.h"
#include <Engine/Geometry/ObjectLoader.h>

using namespace engine;
using namespace std;

void Car::LoadModel(const string &path)
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
