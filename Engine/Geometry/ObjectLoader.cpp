#include <Engine/Geometry/ObjectLoader.h>
#include <Engine/Common/MathFunctions.h>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>

using namespace std;
using namespace glm;

namespace engine
{
    struct Vertex
    {
        Vertex(void)
        {}

        Vertex(const vec3 &position, const vec3 &normal, const vec2 &uv)
            : position(position), normal(normal), uv(uv)
        {}

        bool operator<(const Vertex &other) const
        {
            return memcmp((const void*)this, (const void*)&other, sizeof(Vertex)) > 0;
        }

        bool operator==(const Vertex &other) const
        {
            return Equals(position, other.position) && Equals(normal, other.normal) && Equals(uv, other.uv);
        }

        vec3 position;
        vec3 normal;
        vec2 uv;
    };

    //---------HELPER STUFF---------//
    static inline void InterpolateNormals(vector<Vertex> &outVertices)
    {
         const int n = outVertices.size();
        vector<pair<Vertex, int> > out(n);
        for(int i = 0; i < n; i++)
            out[i] = pair<Vertex, int>(outVertices[i], i);
        sort(out.begin(), out.end());

        int index = 0;
        vec3 interpNor = outVertices[out[0].second].normal;
        for(int i = 1; i < n; i++)
        {
            while(i < n && Equals(out[index].first.position, out[i].first.position))
            {
                interpNor += outVertices[out[i].second].normal;
                i++;
            }
            interpNor = normalize(interpNor);

            for(int j = index; j < i; j++)
                outVertices[out[j].second].normal = interpNor;

            interpNor = vec3(0.0f);
            index = i;
        }
    }

    static inline bool GetSimilarVertexIndex(const Vertex &packed, map<Vertex, GLuint> &vertexToOutIndex, unsigned int &result)
    {
        const auto it = vertexToOutIndex.find(packed);
        if (it == vertexToOutIndex.end())
            return false;
        else
        {
            result = it->second;
            return true;
        }
    }

    static inline void IndexMesh(vector<Vertex> &outVertices, vector<GLuint> &outIndexData)
    {
        const GLuint ctVertices = outVertices.size();

        vector<Vertex> copyVertices(outVertices);
        outVertices.clear();
        outIndexData.reserve(ctVertices);

        map<Vertex, GLuint> vertexToOutIndex;

        for(GLuint i = 0; i < ctVertices; ++i)
        {
            GLuint index;
            bool found = GetSimilarVertexIndex(copyVertices[i], vertexToOutIndex, index);

            if(found)
                outIndexData.push_back(index);
            else
            {
                outVertices.push_back(copyVertices[i]);
                const GLuint ctOutVertices = outVertices.size();
                outIndexData.push_back(ctOutVertices - 1);
                vertexToOutIndex[copyVertices[i]] = ctOutVertices - 1;
            }
        }
    }
    //---------HELPER STUFF---------//



    bool LoadObj(const string &path, const string &filename, Material &mat, TriangleMesh &mesh, bool indexMesh, bool smoothNormals)
    {
        vector<string> coord;
        //vector<unsigned int> indeksi;
        vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        vector<vec3> temp_vertices;
        vector<vec2> temp_uvs;
        vector<vec3> temp_normals;

        ifstream in(path + filename);
        if(!in.is_open())
        {
            cerr << "Cannot open file: " << path + filename << endl;
            return false;
        }
        char buf[256];
        while(!in.eof())
        {
            in.getline(buf,256);
            coord.push_back(buf);
        }
        for(unsigned int i=0;i<coord.size();i++)
        {

            if(coord[i][0]=='v' && coord[i][1]==' ')
            {
                vec3 tmpvertex;
                sscanf(coord[i].c_str(),"v %f %f %f",&tmpvertex.x,&tmpvertex.y,&tmpvertex.z);
                temp_vertices.push_back(tmpvertex);
            }
            else if(coord[i][0]=='v' && coord[i][1]=='n')
            {
                vec3 tmpnormals;
                sscanf(coord[i].c_str(),"vn %f %f %f",&tmpnormals.x,&tmpnormals.y,&tmpnormals.z);
                temp_normals.push_back(tmpnormals);
            }
            else if(coord[i][0]=='v' && coord[i][1]=='t')
            {
                vec2 tmpuv;
                sscanf(coord[i].c_str(),"vt %f %f",&tmpuv.x,&tmpuv.y);
                temp_uvs.push_back(tmpuv);
            }
            else if(coord[i][0]=='m' && coord[i][1]=='t' && coord[i][2]=='l' && coord[i][3]=='l')
            {
                char filen[200];
                sscanf(coord[i].c_str(),"mtllib %s",filen);
                ifstream mtlin(path + string(filen));
                if(!mtlin.is_open())
                {
                    cerr << "Cannot open the material file." << endl;
                    return false;
                }
                vector<string> tmp;
                char c[200];
                while(!mtlin.eof())
                {
                    mtlin.getline(c,200);
                    tmp.push_back(c);
                }
                char mtlname[200];
                char texname[200];
                float amb[3],dif[3],spec[3],alpha,ns,ni;
                int illum;
                bool ismat=false;
                strcpy(texname,"\0");
                for(unsigned int i=0;i<tmp.size();i++)
                {
                    if(tmp[i][0]=='#')
                        continue;
                    if(tmp[i][0]=='n' && tmp[i][1]=='e' && tmp[i][2]=='w')
                    {
                        sscanf(tmp[i].c_str(),"newmtl %s",mtlname);
                    }
                    else if(tmp[i][0]=='N' && tmp[i][1]=='s')
                    {
                        sscanf(tmp[i].c_str(),"Ns %f",&ns);
                        ismat=true;
                    }
                    else if(tmp[i][0]=='K' && tmp[i][1]=='a')
                    {
                        sscanf(tmp[i].c_str(),"Ka %f %f %f",&amb[0],&amb[1],&amb[2]);
                        ismat=true;
                    }
                    else if(tmp[i][0]=='K' && tmp[i][1]=='d')
                    {
                        sscanf(tmp[i].c_str(),"Kd %f %f %f",&dif[0],&dif[1],&dif[2]);
                        ismat=true;
                    }
                    else if(tmp[i][0]=='K' && tmp[i][1]=='s')
                    {
                        sscanf(tmp[i].c_str(),"Ks %f %f %f",&spec[0],&spec[1],&spec[2]);
                        ismat=true;
                    }
                    else if(tmp[i][0]=='N' && tmp[i][1]=='i')
                    {
                        sscanf(tmp[i].c_str(),"Ni %f",&ni);
                        ismat=true;
                    }
                    else if(tmp[i][0]=='d' && tmp[i][1]==' ')
                    {
                        sscanf(tmp[i].c_str(),"d %f",&alpha);
                        ismat=true;
                    }
                    else if(tmp[i][0]=='i' && tmp[i][1]=='l')
                    {
                        sscanf(tmp[i].c_str(),"illum %d",&illum);
                        ismat=true;
                    }
                    else if(tmp[i][0]=='m' && tmp[i][1]=='a')
                    {
                        sscanf(tmp[i].c_str(),"map_Kd %s",texname);
                        ismat=true;
                    }
                }
                if(ismat)
                {
                    if(strcmp(texname,"\0")!=0)
                    {
                        mat.name = mtlname;
                        mat.diffuseColor.x = dif[0];
                        mat.diffuseColor.y = dif[1];
                        mat.diffuseColor.z = dif[2];
                        mat.specularColor.x = spec[0];
                        mat.specularColor.y = spec[1];
                        mat.specularColor.z = spec[2];
                        mat.ambientColor.x = amb[0];
                        mat.ambientColor.y = amb[1];
                        mat.ambientColor.z = amb[2];
                        mat.shininess = ns;
                        mat.diffuse_tex.LoadFromFile((path + texname).c_str());
                    }
                    else
                    {
                        mat.name = mtlname;
                        mat.diffuseColor.x = dif[0];
                        mat.diffuseColor.y = dif[1];
                        mat.diffuseColor.z = dif[2];
                        mat.specularColor.x = spec[0];
                        mat.specularColor.y = spec[1];
                        mat.specularColor.z = spec[2];
                        mat.ambientColor.x = amb[0];
                        mat.ambientColor.y = amb[1];
                        mat.ambientColor.z = amb[2];
                        mat.shininess = ns;
                    }
                }
            }
            else if (coord[i][0]=='f')
            {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                if(coord[i].find("//")!=std::string::npos)
                {
                    sscanf(coord[i].c_str(),"f %d//%d %d//%d %d//%d",&vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    uvIndices.push_back(0);
                    uvIndices.push_back(0);
                    uvIndices.push_back(0);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                }
                else if(coord[i].find("/")!=std::string::npos)
                {
                    sscanf(coord[i].c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    uvIndices.push_back(uvIndex[0]);
                    uvIndices.push_back(uvIndex[1]);
                    uvIndices.push_back(uvIndex[2]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                }
                else
                {
                    sscanf(coord[i].c_str(),"f %d %d %d",&vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    cerr << "NO normals" << endl;
                    return false;
                }
            }
        }

        //load default tex if model doesnt have one
        if(!mat.diffuse_tex.isAlive())
            mat.diffuse_tex.LoadDefault(mat.diffuseColor);

        //unwrap the indexing
        vector<Vertex> outVertices;
        for(int i = 0; i < vertexIndices.size(); ++i)
        {
            const unsigned int vertexInd = vertexIndices[i];
            const unsigned int normalsInd = normalIndices[i];
            const unsigned int uvInd = uvIndices[i];
            const vec3 &position = temp_vertices[vertexInd - 1];
            const vec3 &normal = temp_normals[normalsInd - 1];
            const vec2 uv = uvInd ? temp_uvs[uvInd - 1] : vec2(0.0f, 0.0f);
            outVertices.push_back(Vertex(position, normal, uv));
        }

        if(smoothNormals)
        {
            //smooth the normals
            InterpolateNormals(outVertices);
        }

        if(indexMesh)
        {
            //re-index the mesh
            vector<GLuint> outIndices;
            IndexMesh(outVertices, outIndices);
            mesh.indices.SetData(outIndices);
        }

        //fill output mesh
        for(const Vertex &v : outVertices)
        {
            mesh.positions.push_back(v.position);
            mesh.normals.push_back(v.normal);
            mesh.uvs.push_back(v.uv);
        }

        return true;
    }

}

