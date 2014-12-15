#include <Engine/GL/Skybox.h>

using namespace std;
using namespace glm;

namespace engine
{
    const GLfloat Skybox::box[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    const string Skybox::side_names[] = {
        "front",
        "back",
        "bottom",
        "top",
        "left",
        "right"
    };

    const GLenum Skybox::sides[] = {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X
    };


    Skybox::Skybox(void)
        : cubemap(GL_TEXTURE_CUBE_MAP), p("Shaders/skybox")
    {
    }

    void Skybox::Load(const string &path, const string &name, const string &extension)
    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(GLfloat), &box, GL_STATIC_DRAW);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glActiveTexture(GL_TEXTURE2);
        for(int i = 0; i < 6; ++i)
        {
            const string filename = path + name + side_names[i] + extension;
            cubemap.LoadFromFile(filename.c_str(), sides[i], false);
        }
        cubemap.TexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        cubemap.TexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        cubemap.TexParami(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        cubemap.TexParami(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        cubemap.TexParami(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        cubemap.TexParami(GL_TEXTURE_BASE_LEVEL, 0);
        cubemap.TexParami(GL_TEXTURE_MAX_LEVEL, 0);

        p.SetUniform("cube_map", 2);
    }

    void Skybox::CleanUp()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        p.Destroy();
        cubemap.Destroy();
    }

    void Skybox::Draw(const mat4 &VP)
    {
        glDepthMask(GL_FALSE);
        p.Use();
        p.SetUniform("VP", VP);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
    }



}
