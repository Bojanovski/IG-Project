#include <Engine/Core/Renderer.h>

#include <GL/glew.h>
#include <Engine/Engine.h>
#include <Engine/Common/ErrorCheck.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

using namespace glm;


namespace engine
{
	Renderer::Renderer(void)
        : _2Dprogram("Shaders/sprite"), _3Dprogram("Shaders/DirectionalLight"), _camera(Camera(vec3(3.0f, 0.5f, -5.0f), 4.0f / 3.0f, 60.0f), 4.0f, 0.0025f)
	{
        EventHandler::AddEventListener(&_camera);
        EventHandler::AddUpdateable(&_camera);

		// Create a quad for sprite rendering
		static const GLfloat _quad[8] = {
		0.0f,  0.0f,
		0.0f, -1.0f,
		1.0f,  0.0f,
		1.0f, -1.0f
		};

		// Setup opengl state
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

		// Vertex array object
		glGenVertexArrays(1, &_quad_vao);
		glBindVertexArray(_quad_vao);

		// Vertex buffer object
		glGenBuffers(1, &_quad_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _quad_vbo);

		// Send quad vertex data
		glBufferData(GL_ARRAY_BUFFER, sizeof(_quad)*2, nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_quad), _quad);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_quad), sizeof(_quad), _quad);

		glEnableVertexAttribArray(0);
		GLCheckStmt(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0));
		glEnableVertexAttribArray(1);
		GLCheckStmt(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(_quad)));

        _3Dprogram.Use();
        _3Dprogram.SetUniform("lightDirection", normalize(vec3(0.0f, 1.0f, -1.0f)));
        _3Dprogram.SetUniform("lightIntensity", vec3(1.0f, 1.0f, 1.0f));
        _3Dprogram.SetUniform("textureSampler", 1);

		// Load 2d shader
		_2Dprogram.Use();
		_2Dprogram.SetUniform("color_map", 0);

		// Default screen size
		_size = glm::vec2(640,480);
	}

	void Renderer::SetViewSize(glm::vec2 size)
	{
		_size = size;
	}

	glm::vec2 Renderer::GetViewSize()
	{
		return _size;
	}

	void Renderer::SetClearColor(glm::vec3 color)
	{
		glClearColor(color.x, color.y, color.z, 1.0f);
	}

	void Renderer::RenderSprite(Sprite* sprite, glm::vec2 position, float angle, glm::vec2 scale)
	{
        glActiveTexture(GL_TEXTURE0);

		// Calculate position (Origin is top left corner)
		position.x = 2.0f * position.x - 1.0f;
		position.y = -2.0f * position.y + 1.0f;

		// Get sprite data
		glm::vec2 spr_size = sprite->GetSize();
		glm::vec2 spr_offset = sprite->GetOffset();
		glm::vec2 tex_size = glm::vec2(sprite->GetTexture()->GetWidth(), sprite->GetTexture()->GetHeight());

		// Transform matrix
		glm::mat4 T = glm::mat4(1.0f); // Watch out! Reverse matrix order :)
		T = glm::translate(T, glm::vec3(position, 0.0f)); // End by moving the sprite to the drawing postition
		T = glm::scale(T, glm::vec3(2.0f / _size, 1.0f)); // Scale according to view size
		T = glm::rotate(T, angle, glm::vec3(0.0f, 0.0f, -1.0f)); // Rotate
		T = glm::scale(T, glm::vec3(spr_size * scale, 1.0f)); // Scale
		T = glm::translate(T, glm::vec3(-0.5f, 0.5f, 0.0f)); // First move to the center
		
		// Disable depth and enable alpha blending
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Ready quad vertex data
		glBindVertexArray(_quad_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _quad_vbo);

		// Ready shader
		_2Dprogram.Use();
		_2Dprogram.SetUniform("T", T);
		_2Dprogram.SetUniform("offset", spr_offset/spr_size); // Offset of the sprite image on texture
		_2Dprogram.SetUniform("size", spr_size/tex_size); // Size of the sprite image on texture

		// Ready texture
		sprite->GetTexture()->Bind();
		
		// Draw!
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// Re-enable depth and disable alpha blending
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

    void Renderer::HandleEvent(const SDL_Event &e)
    {
        if(e.type == SDL_WINDOWEVENT)
            if(e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                glViewport(0, 0, e.window.data1, e.window.data2);
                //SetViewSize(glm::vec2(e.window.data1, e.window.data2));
            }
    }

    void Renderer::RenderModel(const Model &model)
    {
        glActiveTexture(GL_TEXTURE1);
        _3Dprogram.Use();

        const mat4 V = _camera.cam.GetViewMatrix();
        const mat4 VP = _camera.cam.GetProjectionMatrix() * V;

        int i = 0;
        for(const TriangleMesh &mesh : model.meshes)
        {
            const auto &mat = model.materials[i];
            mat.diffuse_tex.Bind();

            _3Dprogram.SetUniform("MVP", VP * mesh.transform);
            _3Dprogram.SetUniform("M", mesh.transform);
            _3Dprogram.SetUniform("normalMatrix", transpose(inverse(mat3(mesh.transform))));
            _3Dprogram.SetUniform("invV", inverse(V));

            _3Dprogram.SetUniform("ambient", vec3(mat.ambientColor));
            _3Dprogram.SetUniform("diffuse", vec3(mat.diffuseColor));
            _3Dprogram.SetUniform("specular", vec3(mat.specularColor));
            _3Dprogram.SetUniform("shininess", mat.shininess);

            mesh.Draw();

            ++i;
        }
    }

    void Renderer::CleanUp()
    {
        glDeleteBuffers(1, &_quad_vbo);
        glDeleteVertexArrays(1, &_quad_vao);

        _2Dprogram.Destroy();
        _3Dprogram.Destroy();
    }
}
