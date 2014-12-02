#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 uv;

out vec2 iuv;

uniform mat4 T;

void main()
{
	iuv = uv;
	gl_Position =  T * pos;
}
