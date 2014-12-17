#include "PostProcessAA.h"
#include "GraphicsSettings.h"
#include <iostream>

using namespace engine;
using namespace std;

PostProcessAA::PostProcessAA(GLsizei width, GLsizei height)
    : mFrameBuffer(GL_FRAMEBUFFER, (GLsizei)(width * GraphicsSettings::resolutionScale),  (GLsizei)(height * GraphicsSettings::resolutionScale)),
    mPPProgram("Shaders/PostProcess")
{

}

void PostProcessAA::Init()
{
    mFrameBuffer.AttachDepthBuffer();
    mFrameBuffer.AttachColorTexture();
    if(!mFrameBuffer.CheckStatus())
    {
        cerr << "Exiting because framebuffer is incomplete." << endl;
        exit(-1);
    }

    static const GLfloat quad[] = 
    { 
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        -1.0f, 1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,
    };

    mPPProgram.Use();
    mPPProgram.SetUniform("renderedTexture", 3);
    mPPProgram.SetUniform("useFxaa", GraphicsSettings::FXAA && GraphicsSettings::resolutionScale <= 1.0f);
    mPPProgram.SetUniform("useSsaa", GraphicsSettings::resolutionScale > 1.0f);

    glGenVertexArrays(1, &mQuadVAO);
    glGenBuffers(1, &mQuadVBO);
    glBindVertexArray(mQuadVAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    }
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE3);
    mFrameBuffer.GetTexture().Bind();
}

void PostProcessAA::SetAsRenderTarget() const
{
    mFrameBuffer.Bind();
    glViewport(0, 0, mFrameBuffer.GetWidth(), mFrameBuffer.GetHeight());
}

void PostProcessAA::DrawToScreen(GLsizei screenWidth, GLsizei screenHeight) const
{
    mFrameBuffer.UnBind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE3);
    mPPProgram.Use();

    glBindVertexArray(mQuadVAO);
    glViewport(0, 0, screenWidth, screenHeight);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessAA::Resize(GLsizei width, GLsizei height)
{
    mFrameBuffer.Resize((GLsizei)(width * GraphicsSettings::resolutionScale),  (GLsizei)(height * GraphicsSettings::resolutionScale));
}

void PostProcessAA::CleanUp()
{
    mFrameBuffer.CleanUp();
    mPPProgram.Destroy();
    glDeleteVertexArrays(1, &mQuadVAO);
    glDeleteBuffers(1, &mQuadVBO);
}



