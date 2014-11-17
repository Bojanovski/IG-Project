#version 330 core

in vec2 iuv;

// Ouput data
out vec4 color;

uniform sampler2D color_map;
uniform vec2 offset;
uniform vec2 size;

void main()
{
	color = texture(color_map, (iuv + offset)*size);
}
