/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <iostream>
#include <ctime>
#include <GL/glew.h>
#include <Engine/Engine.h>
#include <Engine/Common/ErrorCheck.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace engine;
using namespace glm;
using namespace std;

void RenderingLoop()
{
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 

    Camera cam(vec3(4.0f, 3.0f, 3.0f), 4.0f / 3.0f, 60.0f);
    DefaultCameraHandler camera(cam, 4.0f, 0.0025f);
    EventHandler::AddEventListener(&camera);
    EventHandler::AddUpdateable(&camera);

    static const GLfloat g_vertex_buffer_data[] = { 
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,

        1.0f,  1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
    };

    static const GLfloat uv_data[] = { 
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data) + sizeof(uv_data), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(g_vertex_buffer_data), g_vertex_buffer_data);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), sizeof(uv_data), uv_data);

    glEnableVertexAttribArray(0);
    GLCheckStmt(glVertexAttribPointer(
        0,                  // The attribute we want to configure
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (const GLvoid*)0    // array buffer offset
        ));


    glEnableVertexAttribArray(1);
    GLCheckStmt(glVertexAttribPointer(
        1,                  // The attribute we want to configure
        2,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (const GLvoid*)sizeof(g_vertex_buffer_data)    // array buffer offset
        ));


    Program program("SimpleShader");
    program.Use();
    program.SetUniform("color_map", 0);

    Texture trollface;
    glActiveTexture(GL_TEXTURE0);
    trollface.LoadFromFile("temp.png");
    trollface.Bind();
    trollface.GenerateMipmaps();
    trollface.TexParami(GL_TEXTURE_WRAP_S, GL_REPEAT);
    trollface.TexParami(GL_TEXTURE_WRAP_T, GL_REPEAT);
    trollface.TexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    trollface.TexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    trollface.TexParamf(GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);


    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

    do
    {
        const float currtime = clock();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const mat4 P = camera.GetProjectionMatrix();
        const mat4 V = camera.GetViewMatrix();
        const mat4 M = rotate(mat4(1.0f), currtime / 20.0f, vec3(0.0f, 1.0f, 0.0f));
        program.SetUniform("MVP", P * V * M);
        
        glDrawArrays(GL_TRIANGLES, 0, 6); // 6 indices starting at 0 -> 2 triangles

        // Swap buffers
        SDLHandler::SwapBuffers();
        EventHandler::ProcessPolledEvents();
        EventHandler::Update();

    } while(!EventHandler::Quit());

    // Cleanup VBO
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program.id);
    glDeleteVertexArrays(1, &VAO);
}

void Test2DRendererLoop()
{
	// Load texture
	Texture tex;
	tex.LoadFromFile("arrows.png");
	tex.GenerateMipmaps();

	// Sprite definitions
	Sprite spr1(tex); // Light arrow
	spr1.SetOffset(glm::vec2(0, 0));
	spr1.SetSize(glm::vec2(128, 128));

	Sprite spr2(tex); // Dark arrow
	spr2.SetOffset(glm::vec2(128, 0));
	spr2.SetSize(glm::vec2(128, 128));

	Sprite spr3(tex); // Green arrow
	spr3.SetOffset(glm::vec2(0, 128));
	spr3.SetSize(glm::vec2(64, 128));

	Sprite spr4(tex); // Orange arrow
	spr4.SetOffset(glm::vec2(64, 128));
	spr4.SetSize(glm::vec2(64, 128));

	Sprite spr5(tex); // Yellow arrow
	spr5.SetOffset(glm::vec2(128, 128));
	spr5.SetSize(glm::vec2(64, 128));

	Sprite spr6(tex); // Blue arrow
	spr6.SetOffset(glm::vec2(192, 128));
	spr6.SetSize(glm::vec2(64, 128));

	// Render init
	Renderer r;
    EventHandler::AddEventListener(&r);
	r.SetClearColor(glm::vec3(0.2f, 0.2f, 0.2f));
	r.SetViewSize(glm::vec2(640,480)); // Screen size (for proper scaling)

	int i = 0;
	do{
		// Spin (framerate based timing, not accurate, for testing only)
		if (i++ > 360)
			i = 0;

		r.Clear(); // Clear the screen

		// Draw each arrow using RenderSprite(sprite, postition, angle, scale)
		r.RenderSprite(&spr1, glm::vec2(213, 160), i);
		r.RenderSprite(&spr2, glm::vec2(426, 160), -45.0f);
		r.RenderSprite(&spr3, glm::vec2(128, 320), i);
		r.RenderSprite(&spr4, glm::vec2(256, 320));
		r.RenderSprite(&spr5, glm::vec2(385, 320), 45.0f, glm::vec2(1.0f, 2.0f));
		r.RenderSprite(&spr6, glm::vec2(512, 320), 90.0f);

		// Display
		SDLHandler::SwapBuffers();

		// Event stuff
		EventHandler::ProcessPolledEvents();
		EventHandler::Update();

	} while (!EventHandler::Quit());
}

int main(int argc, char *argv[])
{
    SDLHandler::Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDLHandler::CreateWindow(
        "Test",                    // window title
        SDL_WINDOWPOS_UNDEFINED,   // initial x position
        SDL_WINDOWPOS_UNDEFINED,   // initial y position
        640,                       // width, in pixels
        480,                       // height, in pixels
        SDL_WINDOW_OPENGL |        // flags
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_SHOWN           
        );

    SDLHandler::InitGL();
    SDLHandler::PrintSoftwareVersions();

    //RenderingLoop();

	Test2DRendererLoop();

    SDLHandler::CleanUp();
    return 0;
}
