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
	// Load texture
		Texture hud;
		hud.LoadFromFile("../Resources/hud.png");
		hud.GenerateMipmaps();

	// Sprite definitions
		Sprite sgbg; // Speed gauge background
		sgbg.SetTexture(hud);
		sgbg.SetOffset(glm::vec2(0, 0));
		sgbg.SetSize(glm::vec2(256, 256));
		sgbg.SetPosition(glm::vec2(0.15f, 0.85f));
		sgbg.SetScale(glm::vec2(0.50f, 0.50f));

		Sprite sgn; // Speed gauge needle
		sgn.SetTexture(hud);
		sgn.SetOffset(glm::vec2(256, 0));
		sgn.SetSize(glm::vec2(256, 256));
		sgn.SetPosition(glm::vec2(0.15f, 0.85f));
		sgn.SetScale(glm::vec2(0.50f, 0.50f));

	// Player car
		CarModel car;
		car.LoadModel("../Resources/CAR/");

	// Test road
		Model road;
		road.materials.push_back(Material());
		road.meshes.push_back(TriangleMesh());
		Material &mat1 = road.materials[0];
		LoadObj("../Resources/Road/", "road_curve.obj", mat1, road.meshes[0], true, false);
		mat1.diffuse_tex.GenerateMipmaps();
		road.LoadToGPU();

	// Test barrier
		Model barrier;
		barrier.materials.push_back(Material());
		barrier.meshes.push_back(TriangleMesh());
		Material &mat2 = barrier.materials[0];
		LoadObj("../Resources/Objects/", "barrier.obj", mat2, barrier.meshes[0], true, false);
		mat2.diffuse_tex.GenerateMipmaps();
		barrier.LoadToGPU();
		barrier.meshes[0].transform = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.2f, 0.0f)), -45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	// Init renderer
		Renderer r;
		EventHandler::AddEventListener(&r);
		r.SetClearColor(glm::vec3(0.2f, 0.2f, 0.2f));
		r.SetViewSize(glm::vec2(640, 480)); // Screen size (for proper sprite scaling)

	// Add objects
		r.AddSprite(&sgbg);
		r.AddSprite(&sgn);

		r.AddModel(&car.GetModel());
		r.AddModel(&road);
		r.AddModel(&barrier);
    
    // Physics
		World phyWorld;
		EventHandler::AddEventListener(&phyWorld);
		EventHandler::AddUpdateable(&phyWorld);

	// Game loop
	do{

		// Clear the screen
			r.Clear();

		// Physics
			static float a = 0.0f;
			a += 0.001f;
			vec3 pos (0.0f, a, 0.0f);
			car.GetPartTransform(CarPart::CAR_BODY) = phyWorld.GetCar().GetTransform();
			car.GetPartTransform(CarPart::CAR_LF_TIRE) = phyWorld.GetChassis().GetWheelTransform_frontLeft();
			car.GetPartTransform(CarPart::CAR_RF_TIRE) = phyWorld.GetChassis().GetWheelTransform_frontRight();
			car.GetPartTransform(CarPart::CAR_LB_TIRE) = phyWorld.GetChassis().GetWheelTransform_backLeft();
			car.GetPartTransform(CarPart::CAR_RB_TIRE) = phyWorld.GetChassis().GetWheelTransform_backRight();
			float speed = phyWorld.GetCarSpeed();

		// Set speed gauge needle
			if (speed > 160.0f) speed = 160.0f;
			if (speed < 0.0f) speed = 0.0f;
			sgn.SetAngle(-120.0f + 240.0f*speed / 160.0f);
		
		// Display
			r.Render();
			SDLHandler::SwapBuffers();

		// Handle events
			EventHandler::ProcessPolledEvents();
			EventHandler::Update();

	} while (!EventHandler::Quit());

	// Cleaning up
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
