#version 330 core

layout(location = 0) in vec4 v_coord;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_uv;

out vec4 position;  // position of the vertex (and fragment) in world space
out vec3 varyingNormalDirection;  // surface normal vector in world space
out vec2 UV;  // surface normal vector in world space

uniform mat4 MVP;
uniform mat4 M;
uniform mat3 normalMatrix; //m_3x3_inv_transp
 
void main()
{
    position = M * v_coord;
    varyingNormalDirection = normalize(normalMatrix * v_normal);
    UV = v_uv;
    gl_Position = MVP * v_coord;
}
