#include "Terrain.h"

#include <Engine/Geometry/ObjectLoader.h>

using namespace engine;

void Terrain::LoadModel(const std::string &path, const std::string &terrainfn)
{
	model.materials.push_back(Material());
	model.meshes.push_back(TriangleMesh());

	Material &mat = model.materials[0];
	LoadObj(path, terrainfn, mat, model.meshes[0], true, true);
	mat.diffuse_tex.GenerateMipmaps();
	mat.diffuse_tex.TexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	mat.diffuse_tex.TexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	mat.diffuse_tex.TexParami(GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	mat.diffuse_tex.TexParami(GL_TEXTURE_WRAP_T, GL_REPEAT);
	mat.diffuse_tex.TexParami(GL_TEXTURE_WRAP_S, GL_REPEAT);

	model.LoadToGPU();
}

engine::Model& Terrain::GetModel()
{
	return model;
}

const engine::Model& Terrain::GetModel() const
{
	return model;
}