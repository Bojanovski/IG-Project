#include <Engine/GL/FrameBuffer.h>
#include <iostream>

using namespace std;

#define TOSTRING(X) (#X)

namespace engine
{
    FrameBuffer::FrameBuffer(GLenum target, GLsizei width, GLsizei height)
        : target(target), width(width), height(height), RBO(0)
    {
        glGenFramebuffers(1, &FBO);
    }

    void FrameBuffer::CleanUp()
    {
        glDeleteFramebuffers(1, &FBO);
        if(RBO)
            glDeleteRenderbuffers(1, &RBO);
        fbTexture.Destroy();
    }


    void FrameBuffer::Bind() const
    {
        glBindFramebuffer(target, FBO);
    }

    void FrameBuffer::UnBind() const
    {
        glBindFramebuffer(target, 0);
    }

    void FrameBuffer::AttachDepthBuffer()
    {
        if(RBO == 0)
        {
            glGenRenderbuffers(1, &RBO);

            Bind();
            glBindRenderbuffer(GL_RENDERBUFFER, RBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            UnBind();
        }

    }

    void FrameBuffer::AttachDepthTexture()
    {
        if(!fbTexture.isAlive())
        {
            fbTexture.LoadEmpty(width, height, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT);
            fbTexture.TexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            fbTexture.TexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            fbTexture.TexParami(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            fbTexture.TexParami(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            Bind();
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, fbTexture.GetID(), 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            UnBind();
        }
    }

    void FrameBuffer::AttachColorTexture()
    {
        if(!fbTexture.isAlive())
        {
            fbTexture.LoadEmpty(width, height, GL_RGBA8, GL_RGBA);
            fbTexture.TexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            fbTexture.TexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            fbTexture.TexParami(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            fbTexture.TexParami(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            Bind();
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbTexture.GetID(), 0);
            const static GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1, drawBuffers);
            glReadBuffer(GL_NONE);
            UnBind();
        }
    }

    bool FrameBuffer::CheckStatus() const
    {
        Bind();
        const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE)
        {
            cerr << "FrameBuffer incomplete: ";
            switch(status)
            {
            case GL_FRAMEBUFFER_UNDEFINED:
                cerr << TOSTRING(GL_FRAMEBUFFER_UNDEFINED);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                cerr << TOSTRING(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                cerr << TOSTRING(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                cerr << TOSTRING(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                cerr << TOSTRING(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER);
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                cerr << TOSTRING(GL_FRAMEBUFFER_UNSUPPORTED);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                cerr << TOSTRING(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                cerr << TOSTRING(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS);
                break;
            default:
                cerr << "Unknown error";
                break;
            }
            cerr << endl;
        }
        UnBind();
        return status == GL_FRAMEBUFFER_COMPLETE;
    }

    bool FrameBuffer::isAlive() const
    {
        return FBO > 0;
    }

    GLsizei FrameBuffer::GetWidth() const
    {
        return width;
    }

    GLsizei FrameBuffer::GetHeight() const
    {
        return height;
    }

    const Texture& FrameBuffer::GetTexture() const
    {
        return fbTexture;
    }

    void FrameBuffer::Resize(GLsizei width, GLsizei height)
    {
        this->width = width;
        this->height = height;
        if(fbTexture.isAlive())
        {
            fbTexture.LoadEmpty(width, height, GL_RGBA8, GL_RGBA);
        }
        if(RBO)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, RBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
    }
};
