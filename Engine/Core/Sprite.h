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

		// Sprite information
		Texture* GetTexture() const;
		void SetTexture(Texture& texture);
		glm::vec2 GetSize() const;
		void SetSize(glm::vec2 size);
		glm::vec2 GetOffset() const;
		void SetOffset(glm::vec2 offset);

		// Transformations
		glm::vec2 Sprite::GetPosition() const;
		void Sprite::SetPosition(glm::vec2 position);
		glm::vec2 Sprite::GetScale() const;
		void Sprite::SetScale(glm::vec2 scale);
		float Sprite::GetAngle() const;
		void Sprite::SetAngle(float angle);

	private:
		Texture* _texture;
		glm::vec2 _size;
		glm::vec2 _offset;

		glm::vec2 _position;
		glm::vec2 _scale;
		float _angle;
	};
}

#endif // EN_SPRITE_H