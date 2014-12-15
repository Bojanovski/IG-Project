#version 330 core

in vec3 texcoords;
uniform samplerCube cube_map;
out vec4 color;

void main()
{
    color = texture(cube_map, texcoords);
}