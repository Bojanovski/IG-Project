#ifndef EN_OBJECTLOADER_H
#define EN_OBJECTLOADER_H

#include <Engine/Geometry/TriangleMesh.h>
#include <Engine/Geometry/Material.h>

namespace engine
{
    bool LoadObj(const std::string &path, const std::string &filename, engine::Material &mat, engine::TriangleMesh &mesh);
}

#endif
