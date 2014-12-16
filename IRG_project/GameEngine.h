#ifndef ACG_GAMEENGINE_H
#define ACG_GAMEENGINE_H

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

class GameEngine
{
public:
    GameEngine(void);
    ~GameEngine(void);

    void Init();
    void CleanUp();
    void RenderingLoop();

private:
    engine::Renderer r;

    //2D
    engine::Texture hud;
    engine::Sprite sgbg; // Speed gauge background
    engine:: Sprite sgn; // Speed gauge needle

    //3D
    RacingTrack rt;
    CarModel car;

    //physics
    engine_physics::World phyWorld;

    //sound
    irrklang::ISoundEngine* soundEngine;
    irrklang::ISound* zvukMotora;
    irrklang::ISound* skripanje;
    irrklang::ISoundSource* zavrsetakSkripanja;


};

#endif
