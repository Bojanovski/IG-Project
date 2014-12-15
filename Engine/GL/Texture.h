#ifndef EN_TEXTURE_H
#define EN_TEXTURE_H

#include <GL/glew.h>

namespace engine
{
    class Texture
    {
    public:
        Texture(GLenum target = GL_TEXTURE_2D);
        
        bool isAlive() const;

        void Bind() const;
        void UnBind() const;

        void LoadFromFile(const char *filename, GLenum _target = GL_TEXTURE_2D, bool flip = true);
        void LoadDefault();
        void Destroy();

        void GenerateMipmaps() const;
        void TexParami(GLenum paramName, GLuint param) const;
        void TexParamf(GLenum paramName, GLfloat param) const;
        void TexParamIuiv(GLenum paramName, const GLuint *param) const;
        void TexParamIiv(GLenum paramName, const GLint *param) const;
        void TexParamfv(GLenum paramName, const GLfloat *param) const;

		int GetWidth();
		int GetHeight();

    private:
        GLuint ID;
        GLenum target;

		int width;
		int height;
    };
}

#endif
