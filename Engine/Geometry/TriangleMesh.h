#ifndef EN_TRIANGLEMESH_H
#define EN_TRIANGLEMESH_H

#include <GL/glew.h>
#include <Engine/Geometry/IndexContainer.h>
#include <glm/glm.hpp>
#include <vector>

namespace engine
{
	class TriangleMesh
	{
	public:
		TriangleMesh(void);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		IndexContainer indices;

		GLenum orientation;
	};
}

#endif
