#version 330 core

layout(location = 0) in vec3 vp;
uniform mat4 VP;
uniform float scale = 500.0;
out vec3 texcoords;

void main()
{
    texcoords = vp;
    gl_Position = VP * vec4(scale * vp, 1.0);
}