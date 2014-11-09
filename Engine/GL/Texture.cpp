#include <Engine/GL/Texture.h>
#include <Engine/Common/ErrorCheck.h>
#include <iostream>
#include <string>
#include <IL/ilu.h>

using namespace std;

namespace engine
{
    void Texture::LoadFromFile(const char *filename)
    {
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
        if(ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
            iluFlipImage();

        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

        if(!success)
        {
            error = ilGetError();
            cout << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << endl;
            exit(-1);
        }

        GLCheckStmt(glGenTextures(1, &ID));
        GLCheckStmt(Bind());
        GLCheckStmt(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData()));

        ilDeleteImages(1, &imageID);
    }

    void Texture::UnBind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    void Texture::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    void Texture::GenerateMipmaps() const
    {
        Bind();
        glGenerateMipmap(GL_TEXTURE_2D);
    }


    void Texture::TexParami(GLenum paramName, GLuint param) const
    {
        Bind();
        glTexParameteri(GL_TEXTURE_2D, paramName, param);
    }

    void Texture::TexParamf(GLenum paramName, GLfloat param) const
    {
        Bind();
        glTexParameterf(GL_TEXTURE_2D, paramName, param);
    }

    void Texture::TexParamIuiv(GLenum paramName, const GLuint *param) const
    {
        Bind();
        glTexParameterIuiv(GL_TEXTURE_2D, paramName, param);
    }

    void Texture::TexParamIiv(GLenum paramName, const GLint *param) const
    {
        Bind();
        glTexParameterIiv(GL_TEXTURE_2D, paramName, param);
    }

    void Texture::TexParamfv(GLenum paramName, const GLfloat *param) const
    {
        Bind();
        glTexParameterfv(GL_TEXTURE_2D, paramName, param);
    }

    void Texture::Destroy()
    {
        glDeleteTextures(1, &ID);
    }



};