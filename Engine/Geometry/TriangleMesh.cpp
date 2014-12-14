#include <Engine/Geometry/TriangleMesh.h>
#include <Engine/Common/ErrorCheck.h>

using namespace glm;

namespace engine
{
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

    void TriangleMesh::DrawInstanced(GLsizei count) const
    {
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, indices.Size(), indices.GetIndexDataType(), 0, count);
    }

    void TriangleMesh::CleanUp()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &IBO);
        glDeleteVertexArrays(1, &VAO);
    }

    float TriangleMesh::GetWidth() const
    {
        float maxx = -FLT_MAX;
        float minx = FLT_MAX;
        for(const vec3 &p : positions)
        {
            maxx = max(maxx, p.x);
            minx = min(minx, p.x);
        }
        return maxx - minx;
    }

    float TriangleMesh::GetHeight() const
    {
        float maxy = -FLT_MAX;
        float miny = FLT_MAX;
        for(const vec3 &p : positions)
        {
            maxy = max(maxy, p.x);
            miny = min(miny, p.x);
        }
        return maxy - miny;
    }

    float TriangleMesh::GetDepth() const
    {
        float maxz = -FLT_MAX;
        float minz = FLT_MAX;
        for(const vec3 &p : positions)
        {
            maxz = max(maxz, p.x);
            minz = min(minz, p.x);
        }
        return maxz - minz;
    }

    float TriangleMesh::GetMaxDim() const
    {
        return max(max(GetWidth(), GetHeight()), GetDepth());
    }
}
