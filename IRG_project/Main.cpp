/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <iostream>
#include <ctime>
#include <GL/glew.h>
#include <Engine/Engine.h>
#include <Engine/Common/ErrorCheck.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Car.h"
#include <Engine\Physics\World.h>

#include <Engine/Geometry/Model.h>
#include <Engine/Geometry/ObjectLoader.h>

using namespace engine;
using namespace engine_physics;
using namespace glm;
using namespace std;

void GameLoop()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);

	// Load texture
	Texture hud;
	hud.LoadFromFile("../Resources/hud.png");
	hud.GenerateMipmaps();

	// Sprite definitions
	Sprite sgbg(hud); // Speed gauge background
	sgbg.SetOffset(glm::vec2(0, 0));
	sgbg.SetSize(glm::vec2(256, 256));

	Sprite sgn(hud); // Speed gauge needle
	sgn.SetOffset(glm::vec2(256, 0));
	sgn.SetSize(glm::vec2(256, 256));

    CarModel car;
    car.LoadModel("../Resources/CAR/");

	// Test road
	Model road;
	road.materials.push_back(Material());
	road.meshes.push_back(TriangleMesh());
	Material &mat = road.materials[0];
	LoadObj("../Resources/Road/", "road_curve.obj", mat, road.meshes[0], true, false);
	mat.diffuse_tex.GenerateMipmaps();
	road.LoadToGPU();

	Renderer r;
	EventHandler::AddEventListener(&r);
	r.SetClearColor(glm::vec3(0.2f, 0.2f, 0.2f));
	r.SetViewSize(glm::vec2(640, 480)); // Screen size (for proper scaling)

	// physics
	World phyWorld;
	EventHandler::AddEventListener(&phyWorld);
	EventHandler::AddUpdateable(&phyWorld);

	do{

		// Clear the screen
		r.Clear();

		static float a = 0.0f;
		a += 0.001f;
		vec3 pos (0.0f, a, 0.0f);
		car.GetPartTransform(CarPart::CAR_BODY) = phyWorld.GetCar().GetTransform();
		car.GetPartTransform(CarPart::CAR_LF_TIRE) = phyWorld.GetChassis().GetWheelTransform_frontLeft();
		car.GetPartTransform(CarPart::CAR_RF_TIRE) = phyWorld.GetChassis().GetWheelTransform_frontRight();
		car.GetPartTransform(CarPart::CAR_LB_TIRE) = phyWorld.GetChassis().GetWheelTransform_backLeft();
		car.GetPartTransform(CarPart::CAR_RB_TIRE) = phyWorld.GetChassis().GetWheelTransform_backRight();
		float speed = phyWorld.GetCarSpeed();

		// Limit needle
		if (speed > 160.0f) speed = 160.0f;
		if (speed < 0.0f) speed = 0.0f;

        r.RenderModel(car.GetModel());
		r.RenderModel(road);

	    // Draw speed gauge
		r.RenderSprite(&sgbg, glm::vec2(0.15f, 0.850f), 0, glm::vec2(0.50f, 0.50f));
		r.RenderSprite(&sgn, glm::vec2(0.15f, 0.85f), -120.0f + 240.0f*speed / 160.0f, glm::vec2(0.50f, 0.50f));

		// Display
		SDLHandler::SwapBuffers();

		// Event stuff
		EventHandler::ProcessPolledEvents();
		EventHandler::Update();

	} while (!EventHandler::Quit());

    hud.Destroy();
    r.CleanUp();
    car.GetModel().CleanUp();
	road.CleanUp();
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

	GameLoop();

    SDLHandler::CleanUp();
    return 0;
}
