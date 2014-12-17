#ifndef ACG_POSTPROCESSAA_H
#define ACG_POSTPROCESSAA_H

#include <glm/glm.hpp>
#include <Engine/GL/FrameBuffer.h>
#include <Engine/GL/Program.h>

//will use SSAA if resolutionScale > 1,
//else FXAA if set,
//else nothing
class PostProcessAA
{
public:
    PostProcessAA(GLsizei width, GLsizei height);

    void Init();
    void SetAsRenderTarget() const;
    void DrawToScreen(GLsizei screenWidth, GLsizei screenHeight) const;
    void CleanUp();

    void Resize(GLsizei width, GLsizei height);

private:
    engine::FrameBuffer mFrameBuffer;
    engine::Program mPPProgram;

    GLuint mQuadVAO;
    GLuint mQuadVBO;
    
};


#endif
