#include "RacingTrack.h"
#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <Engine/Geometry/ObjectLoader.h>
#include <Engine/Common/MathFunctions.h>

using namespace engine;
using namespace std;
using namespace glm;

RacingTrackDescription::RacingTrackDescription(const string &filename)
{
    ifstream in(filename);
    if(!in)
    {
        cerr << "Error opening file: " << filename << endl;
        exit(-1);
    }

    in >> n;
    in >> m;

    //padding
    n += 2;
    m += 2;
    track = new char[n * m];
    memset(track, '.', n * m);

    for(int i = 1; i < n - 1; ++i)
    {
        string line;
        in >> line;
        memcpy((*this)[i]+1, line.c_str(), line.size());
    }
}

RacingTrackDescription::RacingTrackDescription(const RacingTrackDescription &other)
{
    throw exception("RacingTrackDescription can not be copied.");
}

RacingTrackDescription::~RacingTrackDescription(void)
{
    delete[] track;
}

char* RacingTrackDescription::operator[](int k)
{
    return track+(m*k);
}

RacingTrackDescription& RacingTrackDescription::operator=(const RacingTrackDescription &other)
{
    throw exception("RacingTrackDescription can not be copied.");
}

int RacingTrackDescription::Width() const
{
    return m;
}

int RacingTrackDescription::Height() const
{
    return n;
}


RacingTrack::RacingTrack(void)
{
    const vec3 j(0.0f, 1.0f, 0.0f);
    rotations[0] = rotate(I, 0.0f, j);
    rotations[1] = rotate(I, 90.0f, j);
    rotations[2] = rotate(I, 180.0f, j);
    rotations[3] = rotate(I, 270.0f, j);

    layoutToTransform["R..."] = layoutToTransform["R.R."] = pair<mat4, vector<mat4>*>(rotations[0], &straightRoad.transforms);
    layoutToTransform[".R.."] = layoutToTransform[".R.R"] = pair<mat4, vector<mat4>*>(rotations[1], &straightRoad.transforms);
    layoutToTransform["..R."] = layoutToTransform["R.R."] = pair<mat4, vector<mat4>*>(rotations[0], &straightRoad.transforms);
    layoutToTransform["...R"] = layoutToTransform[".R.R"] = pair<mat4, vector<mat4>*>(rotations[1], &straightRoad.transforms);

    layoutToTransform["RR.."] = pair<mat4, vector<mat4>*>(rotations[1], &turnRoad.transforms);
    layoutToTransform[".RR."] = pair<mat4, vector<mat4>*>(rotations[2], &turnRoad.transforms);
    layoutToTransform["..RR"] = pair<mat4, vector<mat4>*>(rotations[3], &turnRoad.transforms);
    layoutToTransform["R..R"] = pair<mat4, vector<mat4>*>(rotations[0], &turnRoad.transforms);

    layoutToTransform[".RRR"] = pair<mat4, vector<mat4>*>(rotations[3], &TRoad.transforms);
    layoutToTransform["R.RR"] = pair<mat4, vector<mat4>*>(rotations[0], &TRoad.transforms);
    layoutToTransform["RR.R"] = pair<mat4, vector<mat4>*>(rotations[1], &TRoad.transforms);
    layoutToTransform["RRR."] = pair<mat4, vector<mat4>*>(rotations[2], &TRoad.transforms);

    layoutToTransform["RRRR"] = pair<mat4, vector<mat4>*>(rotations[0], &crossRoad.transforms);
}

static inline void LoadModel(InstancedModel &model, const string &path, const string &filename)
{
    model.materials.push_back(Material());
    model.meshes.push_back(TriangleMesh());
    Material &mat = model.materials[0];
    LoadObj(path, filename, mat, model.meshes[0], true, false);
    mat.diffuse_tex.GenerateMipmaps();
    mat.diffuse_tex.TexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    mat.diffuse_tex.TexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    mat.diffuse_tex.TexParami(GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
    mat.diffuse_tex.TexParami(GL_TEXTURE_WRAP_T, GL_REPEAT);
    mat.diffuse_tex.TexParami(GL_TEXTURE_WRAP_S, GL_REPEAT);
}

void RacingTrack::LoadModels(const string &path, 
                             const string &straightfn, 
                             const string &turnfn, 
                             const string &Tfn,
                             const string &crossfn,
                             const string &roadBlockfn)
{
    LoadModel(straightRoad, path, straightfn);
    LoadModel(turnRoad, path, turnfn);
    LoadModel(TRoad, path, Tfn);
    LoadModel(crossRoad, path, crossfn);
    LoadModel(roadBlock, path, roadBlockfn);

    roadTileDim = straightRoad.meshes[0].GetMaxDim();
    turnRoadZ = turnRoad.meshes[0].GetDepth();
    turnRoadX = turnRoad.meshes[0].GetWidth();

    roadBlockZ = roadBlock.meshes[0].GetDepth();
    roadBlockX = roadBlock.meshes[0].GetWidth();

    straightRoadZ = straightRoad.meshes[0].GetDepth();
    straightRoadX = straightRoad.meshes[0].GetWidth();
}


void RacingTrack::LoadToGPU()
{
    straightRoad.LoadToGPU();
    roadBlock.LoadToGPU();
    turnRoad.LoadToGPU();
    TRoad.LoadToGPU();
    crossRoad.LoadToGPU();
}


pair<vec2, float> RacingTrack::Create(RacingTrackDescription &rtd)
{
    pair<vec2, float> carTransform;
    const mat4 centering = translate(mat4(1.0f), vec3((float)rtd.Height(), 0.0f, (float)rtd.Width()) * -roadTileDim * 0.5f);

    const int n = rtd.Height() - 1;
    const int m = rtd.Width() - 1;

    //carTransform
    for(int i = 1; i < n; ++i)
    {
        bool found = false;
        for(int j = 1; j < m; ++j)
            if(rtd[i][j] == 'C')
            {
                vec4 pos((float)i * roadTileDim, 0.0f,  (float)j * roadTileDim, 1.0f);
                pos = centering * pos;
                carTransform.first = vec2(pos.x, pos.z);
                carTransform.second = (rtd[i][j-1] == 'R' && rtd[i][j+1] == 'R') ? HALF_PI : 0.0f;

                rtd[i][j] = 'R';
                found = true;
                break;
            }
        if(found)
            break;
    }

    for(int i = 1; i < n; ++i)
        for(int j = 1; j < m; ++j)
            if(rtd[i][j] == 'R')
            {
                const mat4 T = centering * translate(mat4(1.0f), vec3((float)i * roadTileDim, 0.0f, (float)j * roadTileDim));
                const char s[5] = {rtd[i-1][j], rtd[i][j+1], rtd[i+1][j], rtd[i][j-1], '\0'};
                const string str(s);
                if(layoutToTransform.find(str) != layoutToTransform.end())
                {
                    auto &pair = layoutToTransform[str];
                    pair.second->push_back(T * pair.first);
                }
            }


    return carTransform;
}

const InstancedModel* RacingTrack::GetStraightRoad() const
{
    return &straightRoad;
}

const InstancedModel* RacingTrack::GetTurnRoad() const
{
    return &turnRoad;
}

const InstancedModel* RacingTrack::GetRoadBlock() const
{
    return &roadBlock;
}

const engine::InstancedModel* RacingTrack::GetTRoad() const
{
    return &TRoad;
}

const engine::InstancedModel* RacingTrack::GetCrossRoad() const
{
    return &crossRoad;
}

void RacingTrack::CleanUp()
{
    straightRoad.CleanUp();
    turnRoad.CleanUp();
    roadBlock.CleanUp();
    TRoad.CleanUp();
    crossRoad.CleanUp();
}

//void RacingTrack::PlaceRoadBlocks(bool straight, const mat4& T)
//{
//    //No roadblocks for now
//    if(straight)
//    {
//        const int ctBlocks = static_cast<int>(straightRoadX / roadBlockX + 0.5f);
//
//        for(int i = 0; i < ctBlocks; ++i)
//        {
//            const float offsetX = (static_cast<float>(-ctBlocks / 2 + 1 + i) - 0.5f) * roadBlockX;
//
//            const float offsetZ1 = straightRoadZ * 0.5f;
//            const float offsetZ2 = -offsetZ1;
//
//            roadBlock.transforms.push_back(T * translate(I, vec3(offsetX, 0.0f, offsetZ1)));
//            roadBlock.transforms.push_back(T * translate(I, vec3(offsetX, 0.0f, offsetZ2)));
//        }
//    }
//    else
//    {
//        //TODO roadblocks for turns 
//    }
//}
