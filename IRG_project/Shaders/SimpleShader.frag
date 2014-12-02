#version 330 core

in vec2 iuv;

// Ouput data
out vec4 color;

uniform sampler2D color_map;

void main()
{
	color = texture(color_map, iuv);
}
