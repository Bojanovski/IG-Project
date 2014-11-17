#include <Engine/Geometry/ObjectLoader.h>
#include <fstream>
#include <iostream>

namespace engine
{

bool LoadObj(const char *filename, Material &mat, TriangleMesh &mesh)
{
    std::vector<std::string*> coord;
    std::vector<unsigned int> indeksi;

    std::ifstream in(filename);
    if(!in.is_open())
    {
        std::cout << "Nor oepened" << std::endl;
        return false;
    }
    char buf[256];
    while(!in.eof())
    {
        in.getline(buf,256);
        coord.push_back(new std::string(buf));
    }
    for(unsigned int i=0;i<coord.size();i++)
    {
        if((*coord[i])[0]=='v' && (*coord[i])[1]==' ')
        {
            glm::vec3 tmpvertex;
            sscanf(coord[i]->c_str(),"v %f %f %f",&tmpvertex.x,&tmpvertex.y,&tmpvertex.z);
            mesh.positions.push_back(tmpvertex);
        }
        else if((*coord[i])[0]=='v' && (*coord[i])[1]=='n')
        {
            glm::vec3 tmpnormals;
            sscanf(coord[i]->c_str(),"vn %f %f %f",&tmpnormals.x,&tmpnormals.y,&tmpnormals.z);
            mesh.normals.push_back(tmpnormals);
        }
        else if((*coord[i])[0]=='v' && (*coord[i])[1]=='t')
        {
            std::cout<<"ucitavamo uv" << std::endl;
            glm::vec2 tmpuv;
            sscanf(coord[i]->c_str(),"vt %f %f",&tmpuv.x,&tmpuv.y);
            mesh.uvs.push_back(tmpuv);
        }
        else if((*coord[i])[0]=='m' && (*coord[i])[1]=='t' && (*coord[i])[2]=='l' && (*coord[i])[3]=='l')
        {
            char filen[200];
            sscanf(coord[i]->c_str(),"mtllib %s",filen);
            std::ifstream mtlin(filen);
            if(!mtlin.is_open())
            {
                std::cout << "cnnot open the material file" << std::endl;
                return false;
            }
            std::vector<std::string> tmp;
            char c[200];
            while(!mtlin.eof())
            {
                mtlin.getline(c,200);
                tmp.push_back(c);
            }
            char name[200];
            char filename[200];
            float amb[3],dif[3],spec[3],alpha,ns,ni;
            int illum;
            bool ismat=false;
            strcpy(filename,"\0");
            for(unsigned int i=0;i<tmp.size();i++)
            {
                if(tmp[i][0]=='#')
                    continue;
                if(tmp[i][0]=='n' && tmp[i][1]=='e' && tmp[i][2]=='w')
                {
                    sscanf(tmp[i].c_str(),"newmtl %s",name);
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
                    sscanf(tmp[i].c_str(),"map_Kd %s",filename);
                    ismat=true;
                }
            }
            if(ismat)
            {
                if(strcmp(filename,"\0")!=0)
                {
                    mat.name = name;
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
                    mat.diffuse_tex.LoadFromFile(filename);
                }
                else
                {
                    mat.name = name;
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
            std::vector<std::string>().swap(tmp);
        }
        else if ((*coord[i])[0]=='f')
        {
            int a, b, c, d;
            if(coord[i]->find("//")!=std::string::npos)
            {
                sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b);
                indeksi.push_back(a);
                indeksi.push_back(b);
                indeksi.push_back(c);
                indeksi.push_back(b);
                indeksi.push_back(d);
                indeksi.push_back(b);
            }
            else if(coord[i]->find("/")!=std::string::npos)
            {
                int t[3];
                sscanf(coord[i]->c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d",&a,&t[0],&b,&c,&t[1],&b,&d,&t[2],&b);
                indeksi.push_back(a);
                indeksi.push_back(t[0]);
                indeksi.push_back(b);
                indeksi.push_back(c);
                indeksi.push_back(t[1]);
                indeksi.push_back(b);
                indeksi.push_back(d);
                indeksi.push_back(t[2]);
                indeksi.push_back(b);
            }
            else
            {
                sscanf(coord[i]->c_str(),"f %d %d %d",&a,&b,&c);
                indeksi.push_back(a);
                indeksi.push_back(b);
                indeksi.push_back(c);
            }
        }
    }

    mesh.indices.SetData(indeksi);

    std::vector<std::string*>().swap(coord);
    std::vector<unsigned int>().swap(indeksi);

    return true;
}

}

