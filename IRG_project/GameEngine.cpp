#include "GameEngine.h"
#include "GraphicsSettings.h"
#include <glm/glm.hpp>
#include <Engine/Core/EventHandler.h>
#include <Engine/Core/SDLHandler.h>

using namespace engine;
using namespace engine_physics;
using namespace glm;
using namespace std;
using namespace irrklang;

GameEngine::GameEngine(void)
    : 
    isCameraFree(false),
    freeCamera(Camera(vec3(3.0f, 0.5f, -5.0f), (float)GraphicsSettings::windowWidth / (float)GraphicsSettings::windowHeight, GraphicsSettings::FOV), 4.0f, 0.0025f),
    carCamera(Camera(vec3(3.0f, 0.5f, -5.0f), (float)GraphicsSettings::windowWidth / (float)GraphicsSettings::windowHeight, GraphicsSettings::FOV), &phyWorld.GetChassis()),
    postProcess(GraphicsSettings::windowWidth, GraphicsSettings::windowHeight)
{
    EventHandler::AddEventListener(this);

    // Load texture
    hud.LoadFromFile("../Resources/hud.png");
    hud.GenerateMipmaps();

    // Sprite definitions
    sgbg.SetTexture(hud);
    sgbg.SetOffset(vec2(0.0f, 0.0f));
    sgbg.SetSize(vec2(256.0f, 256.0f));
    sgbg.SetPosition(vec2(0.15f, 0.85f));
    sgbg.SetScale(vec2(0.75f, 0.75f));

    sgn.SetTexture(hud);
    sgn.SetOffset(vec2(256.0f, 0.0f));
    sgn.SetSize(vec2(256.0f, 256.0f));
    sgn.SetPosition(vec2(0.15f, 0.85f));
    sgn.SetScale(vec2(0.75f, 0.75f));

    // Init racing track
    rt.LoadModels("../Resources/Road/", "road_str.obj", "road_curve.obj", "barrier.obj");
    const pair<vec2, float> carTransform = rt.Create(RacingTrackDescription("../Resources/TrackDescription.txt"));
    rt.LoadToGPU();

    // Player car
    car.LoadModel("../Resources/CAR/");

	// Terrain
	terrain.LoadModel("../Resources/Terrain/", "terrain.obj");

    // Init renderer
    EventHandler::AddEventListener(&r);
    r.SetClearColor(vec3(0.2f, 0.2f, 0.2f));
	r.SetViewSize(vec2(GraphicsSettings::windowWidth, GraphicsSettings::windowHeight)); // Screen size (for proper sprite scaling)

    // Add objects
    r.AddSprite(&sgbg);
    r.AddSprite(&sgn);

    r.AddModel(&car.GetModel());
	r.AddModel(&terrain.GetModel());
    r.AddInstancedModel(rt.GetRoadBlock());
    r.AddInstancedModel(rt.GetStraightRoad());
    r.AddInstancedModel(rt.GetTurnRoad());

    // Physics
	vector<mat4> sRoads = rt.GetStraightRoad()->transforms;
	vector<mat4> tRoads = rt.GetTurnRoad()->transforms;
    phyWorld.Initialize(carTransform.first, carTransform.second);
	phyWorld.AddStraightRoads(sRoads);
	phyWorld.AddTurnRoads(tRoads);
    EventHandler::AddEventListener(&phyWorld);
    EventHandler::AddUpdateable(&phyWorld);

    // Init cameras
    EventHandler::AddEventListener(&carCamera);
    EventHandler::AddEventListener(&freeCamera);
    EventHandler::AddUpdateable(&carCamera);

    r.SetCamera(&carCamera);

    // Sounds
    // start the sound engine with default parameters
    soundEngine = createIrrKlangDevice();
    if (!soundEngine)
        return; // error starting up the engine

    soundEngine->setListenerPosition(vec3df(0.0f, 0.0f, 0.0f), vec3df(0.0f, 0.0f, 1.0f));
    zvukMotora = soundEngine->play3D("../Resources/Sounds/zvukMotora.ogg", vec3df(0.f, 0.f, 0.f), true, false, true);
    skripanje = soundEngine->play3D("../Resources/Sounds/skripanje.ogg", vec3df(0.f, 0.f, 0.f), true, true, true);
    zavrsetakSkripanja = soundEngine->addSoundSourceFromFile("../Resources/Sounds/zavrsetakSkripanja.ogg");

    if (!(zvukMotora && skripanje && zavrsetakSkripanja))
        return; // error loading sounds
    skripanje->setMinDistance(2.0f);
    zavrsetakSkripanja->setDefaultMinDistance(2.0f);

    postProcess.Init();
}

void GameEngine::RenderingLoop()
{
    bool skripiOdPrije = false;

    // Game loop
    do
    {
        const CameraHandler *camera = r.getCameraHandler();

        // Physics
        car.GetPartTransform(CarPart::CAR_BODY) = phyWorld.GetCar().GetTransform();
        car.GetPartTransform(CarPart::CAR_LF_TIRE) = phyWorld.GetChassis().GetWheelTransform_frontLeft();
        car.GetPartTransform(CarPart::CAR_RF_TIRE) = phyWorld.GetChassis().GetWheelTransform_frontRight();
        car.GetPartTransform(CarPart::CAR_LB_TIRE) = phyWorld.GetChassis().GetWheelTransform_backLeft();
        car.GetPartTransform(CarPart::CAR_RB_TIRE) = phyWorld.GetChassis().GetWheelTransform_backRight();
        float speed = phyWorld.GetCarSpeed();

        // Set speed gauge needle
        if(speed > 160.0f)
            speed = 160.0f;
        if(speed < 0.0f)
            speed = 0.0f;
        sgn.SetAngle(-120.0f + 240.0f * speed * 5 / 160.0f);

        // Display
        postProcess.SetAsRenderTarget();
        r.Clear();
        r.Render();
        postProcess.DrawToScreen(GraphicsSettings::windowWidth, GraphicsSettings::windowHeight);


        SDLHandler::SwapBuffers();

        // Handle events
        EventHandler::ProcessPolledEvents();
        EventHandler::Update();

        // Sound
        // Update listener position and orientation
        const vec3df listenerPositionVec3df(*(vec3df*)&camera->cam.position);
        const vec3df listenerDirectionVec3df(*(vec3df*)&camera->cam.GetDirection());
        const vec3df listenerUpVec3df(*(vec3df*)&camera->cam.GetUp());
        soundEngine->setListenerPosition(listenerPositionVec3df, listenerDirectionVec3df, vec3df(0.0f, 0.0f, 0.0f), listenerUpVec3df);

        // Adjust engine sound
        float speedLimit = phyWorld.getSpeedLimit();
        float brzinaSada = speed;
        zvukMotora->setPlaybackSpeed(brzinaSada / speedLimit + 1.0f);

        // Set position of engine sound
        const vec3df carPositionVec3df(*(vec3df*)&phyWorld.getCarPosition());
        zvukMotora->setPosition(carPositionVec3df);

        // Set screeching tires
        skripanje->setPosition(carPositionVec3df);
        if (phyWorld.isTurning() && (speed / 20.0f > 0.76f)) {
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
}

void GameEngine::CleanUp()
{
    // Cleaning up
    hud.Destroy();
    r.CleanUp();
    car.GetModel().CleanUp();
    rt.CleanUp();
    soundEngine->drop(); // delete engine
    postProcess.CleanUp();
}

void GameEngine::HandleEvent(const SDL_Event &e)
{
    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_c)
    {
        if(isCameraFree)
        {
            EventHandler::RemoveUpdateable(&freeCamera);
            EventHandler::AddUpdateable(&carCamera);
            r.SetCamera(&carCamera);
        }
        else
        {
            EventHandler::RemoveUpdateable(&carCamera);
            EventHandler::AddUpdateable(&freeCamera);
            r.SetCamera(&freeCamera);

            freeCamera.cam.position = carCamera.cam.position;
            freeCamera.cam.phix = carCamera.cam.phix;
            freeCamera.cam.phiy = carCamera.cam.phiy;
            freeCamera.cam.ComputeView();
        }
        isCameraFree = !isCameraFree;
    }
    else if(e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
    {
        GraphicsSettings::windowWidth = e.window.data1;
        GraphicsSettings::windowHeight = e.window.data2;
        postProcess.Resize(GraphicsSettings::windowWidth, GraphicsSettings::windowHeight);
    }
}



