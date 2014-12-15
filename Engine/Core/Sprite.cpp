#include <Engine/Core/Sprite.h>

using namespace glm;

namespace engine
{
	Sprite::Sprite()
	{
		_texture = NULL;
		_size = vec2(0.0f);
		_offset = vec2(0.0f);
		_position = vec2(0.5f);
		_scale = vec2(1.0f);
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

	const vec2& Sprite::GetSize() const
	{
		return _size;
	}

	void Sprite::SetSize(vec2 size)
	{
		_size = size;
	}

	const vec2& Sprite::GetOffset() const
	{
		return _offset;
	}

	void Sprite::SetOffset(vec2 offset)
	{
		_offset = offset;
	}

	const vec2& Sprite::GetPosition() const
	{
		return _position;
	}

	void Sprite::SetPosition(vec2 position)
	{
		_position = position;
	}

	const vec2& Sprite::GetScale() const
	{
		return _scale;
	}

	void Sprite::SetScale(vec2 scale)
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
