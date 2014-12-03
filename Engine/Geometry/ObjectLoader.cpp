#include <Engine/Geometry/ObjectLoader.h>
#include <fstream>
#include <iostream>

using namespace std;
using namespace glm;

namespace engine
{
    bool LoadObj(const string &path, const string &filename, Material &mat, TriangleMesh &mesh)
    {
        vector<string> coord;
        vector<unsigned int> indeksi;
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
            //mesh.positions.push_back(tmpvertex);
        }
        else if(coord[i][0]=='v' && coord[i][1]=='n')
        {
            vec3 tmpnormals;
            sscanf(coord[i].c_str(),"vn %f %f %f",&tmpnormals.x,&tmpnormals.y,&tmpnormals.z);
            temp_normals.push_back(tmpnormals);
            //mesh.normals.push_back(tmpnormals);
        }
        else if(coord[i][0]=='v' && coord[i][1]=='t')
        {
            vec2 tmpuv;
            sscanf(coord[i].c_str(),"vt %f %f",&tmpuv.x,&tmpuv.y);
            temp_uvs.push_back(tmpuv);
            //mesh.uvs.push_back(tmpuv);
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
                    cout << "NO normlas" << endl;
                    return false;
                }
            }
        }
        
        for(unsigned int i=0; i<normalIndices.size(); i++)
	{
    		unsigned int vertexInd = vertexIndices[i];
    		unsigned int normalsInd = normalIndices[i];
    		unsigned int uvInd = uvIndices[i];
		vec3 position = temp_vertices[vertexInd - 1];
		vec3 normal = temp_normals[normalsInd - 1];
    		if(uvInd == 0)
    		{
    			outVertices.push_back(Vertex(position, normal, vec2(0,0)));
    		} else 
    		{
    			vec2 uv = temp_uvs[uvInd - 1];
    			outVertices.push_back(Vertex(position, normal, uv));
    		}
    	}

        return true;
    }

}

