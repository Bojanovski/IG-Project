#ifndef ACG_CAR_H
#define ACG_CAR_H

#include <glm/glm.hpp>
#include <Engine/Geometry/Model.h>

enum CarPart
{
    CAR_BODY = 0,
    CAR_LF_TIRE, //left front
    CAR_RF_TIRE, //right front
    CAR_LB_TIRE, //left back
    CAR_RB_TIRE, //right back
};

class CarModel
{
public:
    void LoadModel(const std::string &path);

    glm::mat4& GetPartTransform(CarPart part);
    const glm::mat4& GetPartTransform(CarPart part) const;

    const engine::Model& GetModel() const;

private:
    engine::Model car;
};


#endif
