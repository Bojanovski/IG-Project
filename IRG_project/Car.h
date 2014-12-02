#ifndef ACG_CAR_H
#define ACG_CAR_H

#include <glm/glm.hpp>
#include <Engine/Geometry/Model.h>

class Car
{
public:
    void LoadModel(const std::string &path);
    engine::Model car;
};


#endif
