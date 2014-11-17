#include <Engine/Core/Sprite.h>

namespace engine
{
	Sprite::Sprite()
	{
		_texture = NULL;
		_size = glm::vec2(0);
		_offset = glm::vec2(0);
	}

	Sprite::Sprite(Texture& texture)
	{
		_texture = &texture;
		_size = texture.GetSize();
		_offset = glm::vec2(0);
	}

	Sprite::Sprite(Texture& texture, glm::vec2 size, glm::vec2 offset)
	{
		_texture = &texture;
		_size = size;
		_offset = offset;
	}

	void Sprite::SetTexture(Texture& texture)
	{
		_texture = &texture;
	}

	void Sprite::SetSize(glm::vec2 size)
	{
		_size = size;
	}

	void Sprite::SetOffset(glm::vec2 offset)
	{
		_offset = offset;
	}

	Texture* Sprite::GetTexture()
	{
		return _texture;
	}

	glm::vec2 Sprite::GetSize()
	{
		return _size;
	}

	glm::vec2 Sprite::GetOffset()
	{
		return _offset;
	}
}