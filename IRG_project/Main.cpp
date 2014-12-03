/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <iostream>
#include <ctime>
#include <GL/glew.h>
#include <Engine/Engine.h>
#include <Engine/Common/ErrorCheck.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Car.h"

using namespace engine;
using namespace glm;
using namespace std;

void Test2DRendererLoop()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS); 

	// Load texture
	Texture tex;
	tex.LoadFromFile("../Resources/arrows.png");
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

    CarModel car;
    car.LoadModel("../Resources/CAR/");

	// Render init
	Renderer r;
    EventHandler::AddEventListener(&r);
	r.SetClearColor(glm::vec3(0.2f, 0.2f, 0.2f));
	r.SetViewSize(glm::vec2(800,600)); // Screen size (for proper scaling)

	int i = 0;
	do{
		// Spin (framerate based timing, not accurate, for testing only)
		if (i++ > 360)
			i = 0;

		r.Clear(); // Clear the screen

        r.RenderModel(car.GetModel());

		// Draw corner arrows
		r.RenderSprite(&spr1, glm::vec2(0.1f, 0.1f), 315);
		r.RenderSprite(&spr1, glm::vec2(0.1f, 0.9f), 225);
		r.RenderSprite(&spr1, glm::vec2(0.9f, 0.1f),  45);
		r.RenderSprite(&spr1, glm::vec2(0.9f, 0.9f), 135);

		// Draw the rest of the arrows
		r.RenderSprite(&spr3, glm::vec2(0.1f, 0.5f));
		r.RenderSprite(&spr4, glm::vec2(0.3f, 0.5f), 0, glm::vec2(1.0f, -1.0f)); // Flipped horizontally
		r.RenderSprite(&spr2, glm::vec2(0.5f, 0.5f), i); // Rotate in the middle of the screen
		r.RenderSprite(&spr5, glm::vec2(0.7f, 0.5f), 45.0f, glm::vec2(1.0f, 2.0f)); // Scaled and at an angle
		r.RenderSprite(&spr6, glm::vec2(0.9f, 0.5f), 90.0f); // Turned sideways

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

	Test2DRendererLoop();

    SDLHandler::CleanUp();
    return 0;
}
