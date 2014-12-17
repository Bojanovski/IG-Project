#ifndef ACG_GAMEENGINE_H
#define ACG_GAMEENGINE_H

#include <Engine/Core/Renderer.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Physics/World.h>
#include <irrKlang.h>
#include "Car.h"
#include "CarCameraHandler.h"
#include "RacingTrack.h"


class GameEngine
    : public engine::EventListener
{
public:
    GameEngine(void);

    void RenderingLoop();
    void CleanUp();
    void HandleEvent(const SDL_Event &e);

private:
    bool isCameraFree;

    engine::Renderer r;
    //2D
    engine::Texture hud;
    engine::Sprite sgbg; // Speed gauge background
    engine:: Sprite sgn; // Speed gauge needle

    //3D
    RacingTrack rt;
    CarModel car;
    engine::DefaultCameraHandler freeCamera;
    CarCameraHandler carCamera;

    //physics
    engine_physics::World phyWorld;

    //sound
    irrklang::ISoundEngine* soundEngine;
    irrklang::ISound* zvukMotora;
    irrklang::ISound* skripanje;
    irrklang::ISoundSource* zavrsetakSkripanja;
};

#endif
