/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef ACG_GRAPHICSSETTINGS_H
#define ACG_GRAPHICSSETTINGS_H

#include <vector>
#include <string>

struct GraphicsSettings
{
    static void Init(const std::string &filename);

    static int windowWidth;
    static int windowHeight;
    static float FOV;
    static bool FXAA;
    static float resolutionScale;
};

#endif
