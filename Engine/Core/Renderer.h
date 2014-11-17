#ifndef EN_RENDERER_H
#define EN_RENDERER_H

#include <glm/glm.hpp>
#include <Engine/Core/Camera.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/GL/Program.h>
#include <Engine/Core/Sprite.h>

namespace engine
{
	class Renderer
        : public EventListener
	{
	public:
		Renderer();
		~Renderer();

		// Draw sprite at screen percent position
		void RenderSprite(Sprite* sprite, glm::vec2 position, float angle = 0.0f, glm::vec2 scale = glm::vec2(1.0f));

		void SetViewSize(glm::vec2 size);
		glm::vec2 GetViewSize();

		void SetClearColor(glm::vec3 color);
		void Clear();

        void HandleEvent(const SDL_Event &e);


	private:
		// Camera
		Camera* _cam;
		DefaultCameraHandler* _camera;

		// View
		glm::vec2 _size;

		// Sprite rendering stuff
		GLuint _quad_vao;
		GLuint _quad_vbo;
		Program* _2Dprogram;
	};
}

#endif // EN_RENDERER_H