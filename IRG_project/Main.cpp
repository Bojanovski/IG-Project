/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/SDLHandler.h>
#include "GameEngine.h"
#include "GraphicsSettings.h"

using namespace engine;
using namespace engine_physics;
using namespace glm;
using namespace std;
using namespace irrklang;

int main(int argc, char *argv[])
{
    GraphicsSettings::Init("../Resources/GraphicsSettings.txt");

    SDLHandler::Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDLHandler::CreateWindow(
        "Test",                         // window title
        SDL_WINDOWPOS_UNDEFINED,        // initial x position
        SDL_WINDOWPOS_UNDEFINED,        // initial y position
        GraphicsSettings::windowWidth,  // width, in pixels
        GraphicsSettings::windowHeight, // height, in pixels
        SDL_WINDOW_OPENGL |             // flags
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_SHOWN           
        );

    SDLHandler::InitGL();
    SDLHandler::PrintSoftwareVersions();

    GameEngine gameEngine;
    gameEngine.RenderingLoop();
    gameEngine.CleanUp();

    SDLHandler::CleanUp();
    return 0;
}
