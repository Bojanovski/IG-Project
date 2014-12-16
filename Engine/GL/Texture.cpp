#include <Engine/GL/Texture.h>
#include <Engine/Common/ErrorCheck.h>
#include <iostream>
#include <string>
#include <IL/ilu.h>

using namespace std;

namespace engine
{
    Texture::Texture(GLenum target)
       : ID(0), target(target)
    {
    }

    void Texture::LoadFromFile(const char *filename, GLenum _target, bool flip)
    {
        //skip if it already exists
        if(ID && _target == GL_TEXTURE_2D)
            return;

        ILuint imageID;
        ILboolean success;
        ILenum error;
        ilGenImages(1, &imageID);
        ilBindImage(imageID);
        success = ilLoadImage(filename);

        if(!success)
        {
            error = ilGetError();
            cerr << "Image load failed " + string(filename) + " - IL reports error: " << error << " - " << iluErrorString(error) << endl;
            exit(-1);
        }

        ILinfo ImageInfo;
        iluGetImageInfo(&ImageInfo);
        if(flip)
            iluFlipImage();

        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

        if(!success)
        {
            error = ilGetError();
            cerr << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << endl;
            exit(-1);
        }

        if(isAlive() == 0)
        {
            GLCheckStmt(glGenTextures(1, &ID));
        }
        GLCheckStmt(Bind());
        GLCheckStmt(glTexImage2D(_target, 0, GL_RGBA8, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData()));

        width = ilGetInteger(IL_IMAGE_WIDTH);
        height = ilGetInteger(IL_IMAGE_HEIGHT);

        ilDeleteImages(1, &imageID);
    }

    void Texture::LoadDefault(const glm::vec4 &color)
    {
        //skip if it already exists
        if(ID)
            return;

        const GLubyte white[4] =
        {
            static_cast<GLubyte>(color.r * 255.0f),
            static_cast<GLubyte>(color.g * 255.0f),
            static_cast<GLubyte>(color.b * 255.0f),
            static_cast<GLubyte>(color.a * 255.0f)
        };

        GLCheckStmt(glGenTextures(1, &ID));
        GLCheckStmt(Bind());
        GLCheckStmt(glTexImage2D(target, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white));
    }

    void Texture::UnBind() const
    {
        glBindTexture(target, 0);
    }

    void Texture::Bind() const
    {
        glBindTexture(target, ID);
    }

    void Texture::GenerateMipmaps() const
    {
        glGenerateMipmap(target);
    }


    void Texture::TexParami(GLenum paramName, GLuint param) const
    {
        glTexParameteri(target, paramName, param);
    }

    void Texture::TexParamf(GLenum paramName, GLfloat param) const
    {
        glTexParameterf(target, paramName, param);
    }

    void Texture::TexParamIuiv(GLenum paramName, const GLuint *param) const
    {
        glTexParameterIuiv(target, paramName, param);
    }

    void Texture::TexParamIiv(GLenum paramName, const GLint *param) const
    {
        glTexParameterIiv(target, paramName, param);
    }

    void Texture::TexParamfv(GLenum paramName, const GLfloat *param) const
    {
        glTexParameterfv(target, paramName, param);
    }

    void Texture::Destroy()
    {
        glDeleteTextures(1, &ID);
        ID = 0;
    }

    int Texture::GetWidth()
    {
        return width;
    }

    int Texture::GetHeight()
    {
        return height;
    }

    bool Texture::isAlive() const
    {
        return ID > 0;
    }

};
