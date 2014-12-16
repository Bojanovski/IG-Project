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
}

void RacingTrack::LoadModels(const string &path, const string &straightfn, const string &turnfn, const string &roadBlockfn)
{
    straightRoad.materials.push_back(Material());
    straightRoad.meshes.push_back(TriangleMesh());
    Material &mat1 = straightRoad.materials[0];
    LoadObj(path, straightfn, mat1, straightRoad.meshes[0], true, false);
    mat1.diffuse_tex.GenerateMipmaps();
    mat1.diffuse_tex.TexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    mat1.diffuse_tex.TexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    mat1.diffuse_tex.TexParami(GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
    mat1.diffuse_tex.TexParami(GL_TEXTURE_WRAP_T, GL_REPEAT);
    mat1.diffuse_tex.TexParami(GL_TEXTURE_WRAP_S, GL_REPEAT);

    turnRoad.materials.push_back(Material());
    turnRoad.meshes.push_back(TriangleMesh());
    Material &mat2 = turnRoad.materials[0];
    LoadObj(path, turnfn, mat2, turnRoad.meshes[0], true, false);
    mat2.diffuse_tex.GenerateMipmaps();
    mat2.diffuse_tex.TexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    mat2.diffuse_tex.TexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    mat2.diffuse_tex.TexParami(GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
    mat2.diffuse_tex.TexParami(GL_TEXTURE_WRAP_T, GL_REPEAT);
    mat2.diffuse_tex.TexParami(GL_TEXTURE_WRAP_S, GL_REPEAT);

    roadBlock.materials.push_back(Material());
    roadBlock.meshes.push_back(TriangleMesh());
    Material &mat3 = roadBlock.materials[0];
    LoadObj(path, roadBlockfn, mat3, roadBlock.meshes[0], true, false);
    mat3.diffuse_tex.GenerateMipmaps();
    mat3.diffuse_tex.TexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    mat3.diffuse_tex.TexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    mat3.diffuse_tex.TexParami(GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
    mat3.diffuse_tex.TexParami(GL_TEXTURE_WRAP_T, GL_REPEAT);
    mat3.diffuse_tex.TexParami(GL_TEXTURE_WRAP_S, GL_REPEAT);

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
}


pair<vec2, float> RacingTrack::Create(RacingTrackDescription &rtd)
{
    pair<vec2, float> carTransform;
    const mat4 centering = translate(mat4(1.0f), vec3((float)rtd.Height(), 0.0f, (float)rtd.Width()) * roadTileDim * -0.5f);

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
    {
        for(int j = 1; j < m; ++j)
        {
            if(rtd[i][j] == 'R' || rtd[i][j] == 'C')
            {
                const mat4 T = centering * translate(mat4(1.0f), vec3((float)i * roadTileDim, 0.0f, (float)j * roadTileDim));

                if(rtd[i+1][j] == 'R' && rtd[i][j+1] == 'R')
                {
                    const mat4 RT = T * rotations[2];
                    turnRoad.transforms.push_back(RT);
                    PlaceRoadBlocks(false, RT);
                }
                else if(rtd[i+1][j] == 'R' && rtd[i][j-1] == 'R')
                {
                    const mat4 RT = T * rotations[3];
                    turnRoad.transforms.push_back(RT);
                    PlaceRoadBlocks(false, RT);
                }
                else if(rtd[i-1][j] == 'R' && rtd[i][j-1] == 'R')
                {
                    const mat4 RT = T * rotations[0];
                    turnRoad.transforms.push_back(RT);
                    PlaceRoadBlocks(false, RT);
                }
                else if(rtd[i-1][j] == 'R' && rtd[i][j+1] == 'R')
                {
                    const mat4 RT = T * rotations[1];
                    turnRoad.transforms.push_back(RT);
                    PlaceRoadBlocks(false, RT);
                }
                else if(rtd[i][j-1] == 'R' && rtd[i][j+1] == 'R')
                {
                    const mat4 RT = T * rotations[1];
                    straightRoad.transforms.push_back(RT);
                    PlaceRoadBlocks(true, RT);
                }
                else
                {
                    straightRoad.transforms.push_back(T);
                    PlaceRoadBlocks(true, T);
                }
            }
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

void RacingTrack::CleanUp()
{
    straightRoad.CleanUp();
    turnRoad.CleanUp();
    roadBlock.CleanUp();
}

void RacingTrack::PlaceRoadBlocks(bool straight, const mat4& T)
{
    const mat4 I(1.0f);
    if(straight)
    {
        const int ctBlocks = static_cast<int>(straightRoadX / roadBlockX + 0.5f);

        for(int i = 0; i < ctBlocks; ++i)
        {
            const float offsetX = (static_cast<float>(-ctBlocks / 2 + 1 + i) - 0.5f) * roadBlockX;

            const float offsetZ1 = straightRoadZ * 0.5f;
            const float offsetZ2 = -offsetZ1;

            roadBlock.transforms.push_back(T * translate(I, vec3(offsetX, 0.0f, offsetZ1)));
            roadBlock.transforms.push_back(T * translate(I, vec3(offsetX, 0.0f, offsetZ2)));
        }
    }
    else
    {
        //TODO roadblocks for turns 
    }
}
