#include <Engine/Geometry/InstancedModel.h>

using namespace glm;
using namespace std;

namespace engine
{
    void InstancedModel::LoadToGPU()
    {
        Model::LoadToGPU();
        glGenBuffers(1, &transforms_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, transforms_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * transforms.size(), transforms.data(), GL_STATIC_DRAW);

        GLuint location = 3;
        GLint components = 4;
        GLenum type = GL_FLOAT;
        GLboolean normalized = GL_FALSE;
        GLsizei datasize = sizeof(mat4);
        char *pointer = 0;
        GLuint divisor = 1;

        for(const TriangleMesh &mesh : meshes)
        {
            glBindVertexArray(mesh.VAO);
            glBindBuffer(GL_ARRAY_BUFFER, transforms_VBO);
            //you need to do everything for each vertex attribute location
            for(int c = 0; c < 4; ++c)
            {
              glEnableVertexAttribArray(location + c); //location of each column
              glVertexAttribPointer(location + c, components, type, normalized, datasize, pointer + c * sizeof(vec4)); //tell other data
              glVertexAttribDivisor(location + c, divisor); //is it instanced?
            }
        }
    }

    void InstancedModel::CleanUp()
    {
        Model::CleanUp();
        glDeleteBuffers(1, &transforms_VBO);
    }

}
