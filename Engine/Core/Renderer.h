#ifndef EN_RENDERER_H
#define EN_RENDERER_H

#include <glm/glm.hpp>
#include <Engine/Core/Camera.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Geometry/Model.h>
#include <Engine/GL/Program.h>
#include <Engine/Core/Sprite.h>

namespace engine
{
	class Renderer
        : public EventListener
	{
	public:
		Renderer(void);

        void Render();
        void AddModel(const Model *model);

		// Draw sprite at screen percent position
        void RenderSprite(Sprite* sprite, glm::vec2 position, float angle = 0.0f, glm::vec2 scale = glm::vec2(1.0f));

		void SetViewSize(const glm::vec2 &size);
		const glm::vec2& GetViewSize() const;

		void SetClearColor(glm::vec3 color);
		void Clear();

        void HandleEvent(const SDL_Event &e);

        void CleanUp();

	private:
        void GenerateCubemap();
        void RenderModel(const Model *model);

        //models to render
        //Renderer does not own these
        std::vector<const Model*> models;

		DefaultCameraHandler _camera;

		// View
		glm::vec2 _size;

		// Sprite rendering stuff
		GLuint _quad_vao;
		GLuint _quad_vbo;
        Program _2Dprogram;
        Program _3Dprogram;

        //skybox stuff
        GLuint _cube_vao;
        GLuint _cube_vbo;
        Texture cubemap;
	};
}

#endif // EN_RENDERER_H
