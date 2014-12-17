#include "GameEngine.h"
#include <glm/glm.hpp>
#include <Engine/Core/EventHandler.h>
#include <Engine/Core/SDLHandler.h>

using namespace engine;
using namespace engine_physics;
using namespace glm;
using namespace std;
using namespace irrklang;

GameEngine::GameEngine(void)
    : freeCamera(Camera(vec3(3.0f, 0.5f, -5.0f), 1.0f / 1.0f, 60.0f), 4.0f, 0.0025f),
    carCamera(Camera(vec3(3.0f, 0.5f, -5.0f), 1.0f / 1.0f, 60.0f), &phyWorld.GetChassis()),
    isCameraFree(false)
{
    EventHandler::AddEventListener(this);

    // Load texture
    hud.LoadFromFile("../Resources/hud.png");
    hud.GenerateMipmaps();

    // Sprite definitions
    sgbg.SetTexture(hud);
    sgbg.SetOffset(vec2(0, 0));
    sgbg.SetSize(vec2(256, 256));
    sgbg.SetPosition(vec2(0.15f, 0.85f));
    sgbg.SetScale(vec2(0.50f, 0.50f));

    sgn.SetTexture(hud);
    sgn.SetOffset(vec2(256, 0));
    sgn.SetSize(vec2(256, 256));
    sgn.SetPosition(vec2(0.15f, 0.85f));
    sgn.SetScale(vec2(0.50f, 0.50f));

    // Init racing track
    rt.LoadModels("../Resources/Road/", "road_str.obj", "road_curve.obj", "barrier.obj");
    const pair<vec2, float> carTransform = rt.Create(RacingTrackDescription("../Resources/TrackDescription.txt"));
    rt.LoadToGPU();

    // Player car
    car.LoadModel("../Resources/CAR/");

    // Init renderer
    EventHandler::AddEventListener(&r);
    r.SetClearColor(vec3(0.2f, 0.2f, 0.2f));
    r.SetViewSize(vec2(640, 480)); // Screen size (for proper sprite scaling)

    // Add objects
    r.AddSprite(&sgbg);
    r.AddSprite(&sgn);

    r.AddModel(&car.GetModel());
    r.AddInstancedModel(rt.GetRoadBlock());
    r.AddInstancedModel(rt.GetStraightRoad());
    r.AddInstancedModel(rt.GetTurnRoad());

    // Physics
    phyWorld.Initialize(carTransform.first, carTransform.second);
    EventHandler::AddEventListener(&phyWorld);
    EventHandler::AddUpdateable(&phyWorld);

    // Init cameras
    int w, h;
    SDLHandler::GetWindowSize(w, h);
    freeCamera.SetAspectRatio(static_cast<float>(w) / static_cast<float>(h));
    carCamera.SetAspectRatio(static_cast<float>(w) / static_cast<float>(h));

    EventHandler::AddEventListener(&carCamera);
    EventHandler::AddUpdateable(&carCamera);

    r.SetCamera(&carCamera);

    // Sounds
    // start the sound engine with default parameters
    soundEngine = createIrrKlangDevice();
    if (!soundEngine)
        return; // error starting up the engine

    soundEngine->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 1));
    zvukMotora = soundEngine->play3D("../Resources/Sounds/zvukMotora.ogg", vec3df(0.f, 0.f, 0.f), true, false, true);
    skripanje = soundEngine->play3D("../Resources/Sounds/skripanje.ogg", vec3df(0.f, 0.f, 0.f), true, true, true);
    zavrsetakSkripanja = soundEngine->addSoundSourceFromFile("../Resources/Sounds/zavrsetakSkripanja.ogg");

    if (!(zvukMotora && skripanje && zavrsetakSkripanja))
        return; // error loading sounds
    skripanje->setMinDistance(2.0f);
    zavrsetakSkripanja->setDefaultMinDistance(2.0f);

}

void GameEngine::RenderingLoop()
{
    float brzinaSada = 0.f, brzinaPrije = 0.f, speedLimit = 0.f;
    bool skripiOdPrije = false;

    // Game loop
    do
    {
        const CameraHandler *camera = r.getCameraHandler();

        // Clear the screen
        r.Clear();

        // Physics
        static float a = 0.0f;
        a += 0.001f;
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
}

void GameEngine::CleanUp()
{
    // Cleaning up
    hud.Destroy();
    r.CleanUp();
    car.GetModel().CleanUp();
    rt.CleanUp();
    soundEngine->drop(); // delete engine
}

void GameEngine::HandleEvent(const SDL_Event &e)
{
    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_c)
    {
        if(isCameraFree)
        {
            EventHandler::RemoveEventListener(&freeCamera);
            EventHandler::RemoveUpdateable(&freeCamera);
            EventHandler::AddEventListener(&carCamera);
            EventHandler::AddUpdateable(&carCamera);
            r.SetCamera(&carCamera);
        }
        else
        {
            EventHandler::RemoveEventListener(&carCamera);
            EventHandler::RemoveUpdateable(&carCamera);
            EventHandler::AddEventListener(&freeCamera);
            EventHandler::AddUpdateable(&freeCamera);
            r.SetCamera(&freeCamera);

            freeCamera.cam.position = carCamera.cam.position;
            freeCamera.cam.phix = carCamera.cam.phix;
            freeCamera.cam.phiy = carCamera.cam.phiy;
            freeCamera.cam.ComputeView();
        }
        isCameraFree = !isCameraFree;
    }
}



