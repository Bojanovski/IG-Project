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
#include "CarCameraHandler.h"
#include "RacingTrack.h"
#include <Engine\Physics\World.h>

#include "RacingTrack.h"
#include <Engine/Geometry/ObjectLoader.h>

#include <irrKlang.h>

using namespace engine;
using namespace engine_physics;
using namespace glm;
using namespace std;
using namespace irrklang;

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

    // Init racing track
        RacingTrack rt;
        rt.LoadModels("../Resources/Road/", "road_str.obj", "road_curve.obj", "barrier.obj");
        const pair<vec2, float> carTransform = rt.Create(RacingTrackDescription("../Resources/TrackDescription.txt"));
        rt.LoadToGPU();

	// Player car
		CarModel car;
		car.LoadModel("../Resources/CAR/");

	// Init renderer
		Renderer r;
		EventHandler::AddEventListener(&r);
		r.SetClearColor(glm::vec3(0.2f, 0.2f, 0.2f));
		r.SetViewSize(glm::vec2(640, 480)); // Screen size (for proper sprite scaling)

	// Add objects
		r.AddSprite(&sgbg);
		r.AddSprite(&sgn);

		r.AddModel(&car.GetModel());
		r.AddInstancedModel(rt.GetRoadBlock());
		r.AddInstancedModel(rt.GetStraightRoad());
        r.AddInstancedModel(rt.GetTurnRoad());

    // Physics
		World phyWorld(carTransform.first, carTransform.second);
		EventHandler::AddEventListener(&phyWorld);
		EventHandler::AddUpdateable(&phyWorld);

    // Init cameras
        int w, h;
        SDLHandler::GetWindowSize(w, h);
        DefaultCameraHandler freeCamera(Camera(vec3(3.0f, 0.5f, -5.0f), (float)w / (float)h, 60.0f), 4.0f, 0.0025f);
        CarCameraHandler carCamera(Camera(vec3(3.0f, 0.5f, -5.0f), (float)w / (float)h, 60.0f), &phyWorld.GetChassis());

        EventHandler::AddEventListener(&freeCamera);
        EventHandler::AddUpdateable(&freeCamera);
        EventHandler::AddEventListener(&carCamera);
        EventHandler::AddUpdateable(&carCamera);

        r.SetCamera(&carCamera);

	// Sounds
		// start the sound engine with default parameters
		ISoundEngine* soundEngine = createIrrKlangDevice();
		if (!soundEngine)
			return; // error starting up the engine

		soundEngine->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 1));
		ISound* zvukMotora = soundEngine->play3D("../Resources/Sounds/zvukMotora.ogg", vec3df(0.f, 0.f, 0.f), true, false, true);
		ISound* skripanje = soundEngine->play3D("../Resources/Sounds/skripanje.ogg", vec3df(0.f, 0.f, 0.f), true, true, true);
		ISoundSource* zavrsetakSkripanja = soundEngine->addSoundSourceFromFile("../Resources/Sounds/zavrsetakSkripanja.ogg");

		if (!(zvukMotora && skripanje && zavrsetakSkripanja))
			return; // error loading sounds
		bool skripiOdPrije = false;
		skripanje->setMinDistance(2.0f);
		zavrsetakSkripanja->setDefaultMinDistance(2.0f);
		
		float brzinaSada = 0.f, brzinaPrije = 0.f, speedLimit = 0.f;
		const CameraHandler *camera = r.getCameraHandler();

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

        // Sound
		// Update listener position and orientation
            const vec3df listenerPositionVec3df(*(vec3df*)&camera->cam.position);
			const vec3df listenerDirectionVec3df(*(vec3df*)&camera->cam.GetDirection());
			const vec3df listenerUpVec3df(*(vec3df*)&camera->cam.GetUp());
			soundEngine->setListenerPosition(listenerPositionVec3df, listenerDirectionVec3df, vec3df(0, 0, 0), listenerUpVec3df);

		// Adjust engine sound
			speedLimit = phyWorld.getSpeedLimit();
			brzinaSada = speed;
            zvukMotora->setPlaybackSpeed(brzinaSada / speedLimit + 1.0f);

		// Set position of engine sound
			const vec3df carPositionVec3df(*(vec3df*)&phyWorld.getCarPosition());
			zvukMotora->setPosition(carPositionVec3df);

		// Set screeching tires
			skripanje->setPosition(carPositionVec3df);
			if (phyWorld.isTurning() && (speed/20 > 0.76)) {
				if (!skripiOdPrije) {
					skripanje->setIsPaused(false);
					skripiOdPrije = true;
				}
			}
			else if (skripiOdPrije) {
				skripanje->setPlayPosition(0);
				skripanje->setIsPaused(true);
				soundEngine->play3D(zavrsetakSkripanja, carPositionVec3df);
				skripiOdPrije = false;
			}

	} while (!EventHandler::Quit());

	// Cleaning up
    hud.Destroy();
    r.CleanUp();
    car.GetModel().CleanUp();
	rt.CleanUp();
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
        1280,                       // width, in pixels
        720,                       // height, in pixels
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
