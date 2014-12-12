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

#include <irrKlang.h>

using namespace engine;
using namespace engine_physics;
using namespace glm;
using namespace std;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

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

	// Sounds
		// start the sound engine with default parameters
		irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
		if (!soundEngine)
			return; // error starting up the engine

		soundEngine->setListenerPosition(irrklang::vec3df(0, 0, 0), irrklang::vec3df(0, 0, 1));
		/*irrklang::ISound* music = engine->play3D("The_Violation.mp3",
			irrklang::vec3df(0.f, 0.f, 0.f), true, false, true);*/
		irrklang::ISound* zvukMotora = soundEngine->play3D("stoji.ogg", 
			irrklang::vec3df(0.f, 0.f, 0.f), true, false, true);
		
		float brzinaSada = 0.f, brzinaPrije = 0.f, speedLimit = 0.f;
		DefaultCameraHandler* camera = r.getCameraHandler();

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

		// Update listener position and orientation
			vec3 listenerPositionVec3 = (*camera).cam.position;
			irrklang::vec3df listenerPositionVec3df(listenerPositionVec3.x, listenerPositionVec3.y, listenerPositionVec3.z);
			vec3 listenerDirectionVec3 = (*camera).cam.GetDirection();
			irrklang::vec3df listenerDirectionVec3df(listenerDirectionVec3.x, listenerDirectionVec3.y, 	listenerDirectionVec3.z);
			vec3 listenerUpVec3 = (*camera).cam.GetUp();
			irrklang::vec3df listenerUpVec3df(-listenerUpVec3.x, -listenerUpVec3.y, -listenerUpVec3.z);
			soundEngine->setListenerPosition(listenerPositionVec3df, listenerDirectionVec3df, irrklang::vec3df(0, 0, 0), listenerUpVec3df);

		// Adjust engine sound
			speedLimit = phyWorld.getSpeedLimit();
			brzinaSada = speed;
			zvukMotora->setPlaybackSpeed(brzinaSada / speedLimit + 1);

		// Set position of engine sound
			vec3 carPosition = phyWorld.getCarPosition();
			irrklang::vec3df carPositionVec3df(carPosition.x, carPosition.y, carPosition.z);
			zvukMotora->setPosition(carPositionVec3df);

	} while (!EventHandler::Quit());

	// Cleaning up
    hud.Destroy();
    r.CleanUp();
    car.GetModel().CleanUp();
	road.CleanUp();
	//if (music)
		//music->drop(); // release music stream.
	soundEngine->drop(); // delete engine
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
