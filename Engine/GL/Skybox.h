#ifndef EN_SKYBOX_H
#define EN_SKYBOX_H

#include <GL/glew.h>
#include <Engine/GL/Texture.h>
#include <Engine/GL/Program.h>
#include <string>

namespace engine
{
    class Skybox
    {
    public:
        Skybox(void);

        //"left", "right", "top", "bottom", "far" and "near" are added to name for each side
        void Load(const std::string &path, const std::string &name, const std::string &extension);
        void CleanUp();
        void Draw(const glm::mat4 &VP);

    private:
        Texture cubemap;
        Program p;
        GLuint VAO;
        GLuint VBO;

        static const GLfloat box[];
        static const std::string side_names[];
        static const GLenum sides[];
    };
}

#endif
