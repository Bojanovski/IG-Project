#ifndef EN_FRAMEBUFFER_H
#define EN_FRAMEBUFFER_H

#include <GL/glew.h>
#include <Engine/GL/Texture.h>

namespace engine
{
    class FrameBuffer
    {
    public:
        FrameBuffer(GLenum target, GLsizei width, GLsizei height);
        
        void CleanUp();

        void Bind() const;
        void UnBind() const;

        void AttachDepthBuffer();
        void AttachDepthTexture();
        void AttachColorTexture();

        void Resize(GLsizei width, GLsizei height);

        // true - its ok
        bool CheckStatus() const;

        bool isAlive() const;

		GLsizei GetWidth() const;
		GLsizei GetHeight() const;
        const Texture& GetTexture() const;

    private:
        GLuint FBO;
        GLuint RBO;
        Texture fbTexture;
        GLenum target;

		GLsizei width;
		GLsizei height;
    };
}

#endif
