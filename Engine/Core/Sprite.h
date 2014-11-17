#ifndef EN_SPRITE_H
#define EN_SPRITE_H

#include <glm/glm.hpp>
#include <Engine/GL/Texture.h>

namespace engine
{
	class Sprite
	{
	public:
		Sprite();
		Sprite(Texture& texture);
		Sprite(Texture& texture, glm::vec2 size, glm::vec2 offset);

		void SetTexture(Texture& texture);
		void SetSize(glm::vec2 size);
		void SetOffset(glm::vec2 offset);

		Texture* GetTexture();
		glm::vec2 GetSize();
		glm::vec2 GetOffset();

	private:
		Texture* _texture;
		glm::vec2 _size;
		glm::vec2 _offset;
	};
}

#endif // EN_SPRITE_H