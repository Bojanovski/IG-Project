/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <iostream>
#include <GL/glew.h>
#include <Engine/Engine.h>

using namespace engine;
using namespace glm;
using namespace std;

void RenderingLoop()
{
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 

    Camera cam(vec3(4.0f, 3.0f, 3.0f), 4.0f / 3.0f, 60.0f);
    DefaultCameraHandler camera(cam, 4.0f, 0.0025f);
    EventHandler::AddEventListener(&camera);
    EventHandler::AddUpdateable(&camera);

    static const GLfloat g_vertex_buffer_data[] = { 
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // The attribute we want to configure
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (const GLvoid*)0    // array buffer offset
        );

    Program program("SimpleShader");

    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

    do
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use our shader
        program.Use();


        mat4 P = camera.GetProjectionMatrix();
        mat4 V = camera.GetViewMatrix();
        mat4 M(1.0f);
        program.SetUniform("MVP", P * V * M);
        program.SetUniform("normalMatrix", mat3(V * M));
        program.SetUniform("lightPosition_worldspace", vec3(5.0f, 5.0f, 5.0f));

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
        
        glDisableVertexAttribArray(0);

        // Swap buffers
        SDLHandler::SwapBuffers();
        EventHandler::ProcessPolledEvents();
        EventHandler::Update();

    } while(!EventHandler::Quit());

    // Cleanup VBO
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program.id);
    glDeleteVertexArrays(1, &VAO);
}

int main(int argc, char *argv[])
{
    SDLHandler::InitSDL(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

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

    RenderingLoop();

    SDLHandler::CleanUp();
    return 0;
}
