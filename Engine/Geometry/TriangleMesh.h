#ifndef EN_TRIANGLEMESH_H
#define EN_TRIANGLEMESH_H

#include <GL/glew.h>
#include <Engine/Geometry/IndexContainer.h>
#include <Engine/GL/Program.h>
#include <glm/glm.hpp>
#include <vector>

namespace engine
{
	class TriangleMesh
	{
        friend class InstancedModel;
	public:
		TriangleMesh(void);

        GLsizei PositionsSizeInBytes() const;
        GLsizei NormalsSizeInBytes() const;
        GLsizei UVsSizeInBytes() const;
        GLsizei TotalSizeInBytes() const;

        const GLfloat* GetPositions() const;
        const GLfloat* GetNormals() const;
        const GLfloat* GetUVs() const;

        void LoadToGPU();

        //draw the mesh with the currently bound program
        //the program must be bound beforehand
        void Draw() const;
        void DrawInstanced(GLsizei count) const;
        void CleanUp();

        float GetWidth() const; //x
        float GetHeight() const; //y
        float GetDepth() const; //z

        float GetMaxDim() const;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        IndexContainer indices;

        glm::mat4 transform;

    private:
		GLenum orientation;
        GLuint VAO;
        GLuint VBO;
        GLuint IBO;
	};
}

#endif
