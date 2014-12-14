#version 330 core

layout(location = 0) in vec4 v_coord;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_uv;
layout(location = 3) in mat4 v_M;

out vec4 position;  // position of the vertex (and fragment) in world space
out vec3 varyingNormalDirection;  // surface normal vector in world space
out vec2 UV;  // surface normal vector in world space

uniform mat4 MVP;
 
void main()
{
    position = v_M * v_coord;
    varyingNormalDirection = normalize(transpose(inverse(mat3(v_M))) * v_normal);
    UV = v_uv;
    gl_Position = MVP * position;
}