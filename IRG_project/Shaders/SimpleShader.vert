#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec2 uv;

out vec2 iuv;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main()
{
    iuv = uv;
	gl_Position =  MVP * vertexPosition_modelspace;
}
