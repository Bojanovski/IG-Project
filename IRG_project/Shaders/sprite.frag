#version 330 core

in vec2 iuv;

// Ouput data
layout(location = 0) out vec4 color;

uniform sampler2D color_map;
uniform vec2 offset;
uniform vec2 size;

void main()
{
	color = texture(color_map, (iuv + offset)*size);
}
