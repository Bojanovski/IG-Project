#ifndef EN_MATERIAL_H
#define EN_MATERIAL_H

#include <glm/glm.hpp>
#include <string>
#include <Engine/GL/Texture.h>

namespace engine
{
	struct Material
	{
		std::string name;
		glm::vec4 diffuseColor;
		glm::vec4 specularColor;
		glm::vec4 ambientColor;
		GLfloat shininess;

		Texture diffuse_tex;
	};
}

#endif
