#include <Engine/Geometry/TriangleMesh.h>
#include <Engine/Common/ErrorCheck.h>

using namespace glm;

namespace engine
{
    Vertex::Vertex(void)
    {
    }

    Vertex::Vertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &uv)
        : position(position), normal(normal), uv(uv)
    {
    }


	TriangleMesh::TriangleMesh(void)
		: orientation(GL_CCW), transform(1.0f)
	{
	}

    GLsizei TriangleMesh::PositionsSizeInBytes() const
    {
        return positions.size() * sizeof(vec3);
    }

    GLsizei TriangleMesh::NormalsSizeInBytes() const
    {
        return normals.size() * sizeof(vec3);
    }

    GLsizei TriangleMesh::UVsSizeInBytes() const
    {
        return uvs.size() * sizeof(vec2);
    }

    GLsizei TriangleMesh::TotalSizeInBytes() const
    {
        return PositionsSizeInBytes() + NormalsSizeInBytes() + UVsSizeInBytes(); 
    }

    const GLfloat* TriangleMesh::GetPositions() const
    {
        return (const GLfloat*)positions.data();
    }

    const GLfloat* TriangleMesh::GetNormals() const
    {
        return (const GLfloat*)normals.data();
    }

    const GLfloat* TriangleMesh::GetUVs() const
    {
        return (const GLfloat*)uvs.data();
    }

    void TriangleMesh::LoadToGPU()
    {
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &IBO);
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, TotalSizeInBytes(), nullptr, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            GLintptr offset = 0;
            glBufferSubData(GL_ARRAY_BUFFER, offset, PositionsSizeInBytes(), GetPositions());
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)offset);
            offset += PositionsSizeInBytes();

            glBufferSubData(GL_ARRAY_BUFFER, offset, NormalsSizeInBytes(), GetNormals());
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)offset);
            offset += NormalsSizeInBytes();

            glBufferSubData(GL_ARRAY_BUFFER, offset, UVsSizeInBytes(), GetUVs());
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)offset);
            offset += UVsSizeInBytes();

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.GetSizeInBytes(), indices.GetData(), GL_STATIC_DRAW);
        }
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void TriangleMesh::Draw() const
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.Size(), indices.GetIndexDataType(), nullptr);
    }
}
