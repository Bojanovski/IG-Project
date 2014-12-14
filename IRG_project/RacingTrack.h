#ifndef ACG_RACINGTRACK_H
#define ACG_RACINGTRACK_H

#include <glm/glm.hpp>
#include <Engine/Geometry/InstancedModel.h>

class RacingTrackDescription
{
public:
    RacingTrackDescription(const std::string &filename);
    ~RacingTrackDescription(void);

    char* operator[](int k);

    int Width() const;
    int Height() const;

    //not allowed
    RacingTrackDescription(const RacingTrackDescription &other);
    RacingTrackDescription& operator=(const RacingTrackDescription &other);

private:
    char *track;
    int n, m;
};

class RacingTrack
{
public:
    RacingTrack(void);

    void LoadModels(const std::string &path, const std::string &straightfn, const std::string &turnfn, const std::string &roadBlockfn);
    void LoadToGPU();
    void Create(RacingTrackDescription &rtd);
    void CleanUp();

    const engine::InstancedModel* GetStraightRoad() const;
    const engine::InstancedModel* GetTurnRoad() const;
    const engine::InstancedModel* GetRoadBlock() const;

private:
    engine::InstancedModel straightRoad;
    engine::InstancedModel turnRoad;
    engine::InstancedModel roadBlock;

    float roadTileDim;
    glm::mat4 rotations[4];
};


#endif
