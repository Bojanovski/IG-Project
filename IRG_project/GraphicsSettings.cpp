#include "GraphicsSettings.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define CONVERT(N, V, F) N = F(V)

int GraphicsSettings::windowWidth = 1280;
int GraphicsSettings::windowHeight = 720;
float GraphicsSettings::FOV = 60.0f;
bool GraphicsSettings::FXAA = false;
float GraphicsSettings::resolutionScale = 1.0f;

void GraphicsSettings::Init(const std::string &filename)
{
    ifstream f(filename);
    if(f.is_open())
    {
        while(!f.eof())
        {
            string name, value;
            f >> name;
            if(name.substr(0, 1) == "#")
            {
                f >> value;
                if(name == "#windowWidth")
                    CONVERT(windowWidth, value, stoi);
                else if(name == "#windowHeight")
                    CONVERT(windowHeight, value, stoi);
                else if(name == "#FOV")
                    CONVERT(FOV, value, stof);
                else if(name == "#FXAA")
                    CONVERT(FXAA, value, stoi);
                else if(name == "#resolutionScale")
                    CONVERT(resolutionScale, value, stof);
            }
        }
    }
    else
    {
        cerr << "Cannot open graphics settings" << endl;
    }
}
