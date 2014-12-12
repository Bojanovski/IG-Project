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

		void AddSprite(const Sprite* sprite);
        void AddModel(const Model* model);
		
		void Render();
		
		const glm::vec2& GetViewSize() const;
		void SetViewSize(const glm::vec2 &size);
		
		void SetClearColor(glm::vec3 color);
		void Clear();

        void HandleEvent(const SDL_Event &e);

        void CleanUp();

		DefaultCameraHandler* getCameraHandler() { return &_camera; }

	private:
        void GenerateCubemap();
		void RenderSprite(const Sprite* sprite);
        void RenderModel(const Model* model);

        //sprites to render
        //Renderer does not own these
        std::vector<const Sprite*> sprites;

		//models to render
		//Renderer does not own these
		std::vector<const Model*> models;

		// View
		glm::vec2 _size;

		DefaultCameraHandler _camera;

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
