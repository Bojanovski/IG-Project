#ifndef ACG_TERRAIN_H
#define ACG_TERRAIN_H

#include <Engine/Geometry/Model.h>

class Terrain
{
public:
	void LoadModel(const std::string &path, const std::string &terrainfn);

	engine::Model& GetModel();
	const engine::Model& GetModel() const;

private:
	engine::Model model;
};

#endif // ACG_TERRAIN_H