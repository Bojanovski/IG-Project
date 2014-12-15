#ifndef EN_RENDERER_H
#define EN_RENDERER_H

#include <glm/glm.hpp>
#include <Engine/Core/Camera.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Geometry/InstancedModel.h>
#include <Engine/GL/Program.h>
#include <Engine/GL/Skybox.h>
#include <Engine/Core/Sprite.h>

namespace engine
{
    //sprites use texture slot 0
    //models use texture slot 1
    //skybox uses texture slot 2
	class Renderer
        : public EventListener
	{
	public:
		Renderer(void);

		void AddSprite(const Sprite* sprite);
        void AddModel(const Model* model);
        void AddInstancedModel(const InstancedModel* model);
		
		void Render();
		
		const glm::vec2& GetViewSize() const;
		void SetViewSize(const glm::vec2 &size);
		
		void SetClearColor(glm::vec3 color);
		void Clear();

        void HandleEvent(const SDL_Event &e);

        void CleanUp();

		DefaultCameraHandler* getCameraHandler() { return &_camera; }

	private:
		void RenderSprite(const Sprite* sprite);
        void RenderModel(const Model* model);
        void RenderInstancedModel(const InstancedModel* model);

        //skybox
        Skybox skybox;

        //sprites to render
        //Renderer does not own these
        std::vector<const Sprite*> sprites;

		//models to render
		//Renderer does not own these
		std::vector<const Model*> models;
        Program _3Dprogram;
        std::vector<const InstancedModel*> instancedModels;
        Program _3DprogramInstanced;

		// View
		glm::vec2 _size;

		DefaultCameraHandler _camera;

		// Sprite rendering stuff
		GLuint _quad_vao;
		GLuint _quad_vbo;
        Program _2Dprogram;
	};
}

#endif // EN_RENDERER_H
