/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/SDLHandler.h>
#include <Engine/Common/ErrorCheck.h>
#include <iostream>

using namespace std;

namespace engine
{
    SDL_Window *SDLHandler::window;
    SDL_GLContext SDLHandler::opengl_context;

    void SDLHandler::InitSDL(Uint32 flags)
    {
        SDLErrCheck(SDL_Init(flags));
        SDLErrCheck(SDL_SetRelativeMouseMode(SDL_TRUE));
    }

    void SDLHandler::CleanUp()
    {
        SDL_GL_DeleteContext(opengl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void SDLHandler::PrintSoftwareVersions()
    {
        cout << "GLEW version: " << glewGetString(GLEW_VERSION) << endl << endl;

        SDL_version compiled;
        SDL_version linked;

        SDL_VERSION(&compiled);
        SDL_GetVersion(&linked);
        cout << "Compiled against SDL version "  << static_cast<int>(compiled.major) << "." << 
                                                     static_cast<int>(compiled.minor)  << "." <<  
                                                     static_cast<int>(compiled.patch) << endl;
        cout << "Linked against SDL version "  << static_cast<int>(linked.major) << "." << 
                                                   static_cast<int>(linked.minor)  << "." <<  
                                                   static_cast<int>(linked.patch) << endl << endl;

        cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << endl;
        cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << endl;
        cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
        cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl << endl;
    }

    void SDLHandler::CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
    {
        window = SDL_CreateWindow(title, x, y, w, h, flags);
        SDLErrCheck(!window);
    }

    void SDLHandler::InitGL()
    {
        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3));
        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));

        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32));

        // create the opengl context
        opengl_context = SDL_GL_CreateContext(window);
        SDLErrCheck(!opengl_context);
    
        SDLErrCheck(SDL_GL_SetSwapInterval(1));

        glewExperimental = GL_TRUE;
        GLEWErrCheck(glewInit());
        GLErrCheck(false);
    }

    void SDLHandler::SwapBuffers()
    {
        SDL_GL_SwapWindow(window);
    }

    void SDLHandler::GetWindowSize(int &w, int &h)
    {
        SDL_GetWindowSize(window, &w, &h);
    }

    void SDLHandler::SetMousePositionToWindowCenter()
    {
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        SDL_WarpMouseInWindow(window, w / 2, h / 2);
    }
}
