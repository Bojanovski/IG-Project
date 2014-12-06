#include <Engine/Core/Sprite.h>

namespace engine
{
	Sprite::Sprite()
	{
		_texture = NULL;
		_size = glm::vec2(0);
		_offset = glm::vec2(0);
		_position = glm::vec2(0.5f);
		_scale = glm::vec2(1.0f);
		_angle = 0.0f;
	}

	Texture* Sprite::GetTexture() const
	{
		return _texture;
	}

	void Sprite::SetTexture(Texture& texture)
	{
		_texture = &texture;
	}

	glm::vec2 Sprite::GetSize() const
	{
		return _size;
	}

	void Sprite::SetSize(glm::vec2 size)
	{
		_size = size;
	}

	glm::vec2 Sprite::GetOffset() const
	{
		return _offset;
	}

	void Sprite::SetOffset(glm::vec2 offset)
	{
		_offset = offset;
	}

	glm::vec2 Sprite::GetPosition() const
	{
		return _position;
	}

	void Sprite::SetPosition(glm::vec2 position)
	{
		_position = position;
	}

	glm::vec2 Sprite::GetScale() const
	{
		return _scale;
	}

	void Sprite::SetScale(glm::vec2 scale)
	{
		_scale = scale;
	}

	float Sprite::GetAngle() const
	{
		return _angle;
	}

	void Sprite::SetAngle(float angle)
	{
		_angle = angle;
	}
}