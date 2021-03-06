#ifndef ACG_RACINGTRACK_H
#define ACG_RACINGTRACK_H

#include <glm/glm.hpp>
#include <Engine/Geometry/InstancedModel.h>
#include <map>

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

    void LoadModels(const std::string &path, 
        const std::string &straightfn, 
        const std::string &turnfn, 
        const std::string &Tfn,
        const std::string &crossfn,
        const std::string &roadBlockfn);

    void LoadToGPU();
    //returns car transform
    std::pair<glm::vec2, float> Create(RacingTrackDescription &rtd);
    void CleanUp();

    const engine::InstancedModel* GetStraightRoad() const;
    const engine::InstancedModel* GetTurnRoad() const;
    const engine::InstancedModel* GetTRoad() const;
    const engine::InstancedModel* GetCrossRoad() const;
    const engine::InstancedModel* GetRoadBlock() const;

private:
    //void PlaceRoadBlocks(bool straight, const glm::mat4& T);

    engine::InstancedModel straightRoad;
    engine::InstancedModel turnRoad;
    engine::InstancedModel TRoad;
    engine::InstancedModel crossRoad;
    engine::InstancedModel roadBlock;

    float roadTileDim;

    float straightRoadX, straightRoadZ;
    float roadBlockX, roadBlockZ;
    float turnRoadX, turnRoadZ;

    glm::mat4 rotations[4];
    std::map<std::string, std::pair<glm::mat4, std::vector<glm::mat4>* > > layoutToTransform;
};


#endif
