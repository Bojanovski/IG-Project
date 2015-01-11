#version 330 core

in vec4 position;  // position of the vertex (and fragment) in world space
in vec3 varyingNormalDirection;  // surface normal vector in world space
in vec2 UV;  // surface normal vector in world space

// Ouput data
layout(location = 0) out vec3 color;

uniform mat4 invV;
 
//Matrial
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
uniform sampler2D textureSampler;

//Scene
uniform vec3 lightDirection;
uniform vec3 lightIntensity;
uniform vec3 scene_ambient;

void main()
{
    vec3 normalDirection = normalize(varyingNormalDirection);
    vec3 viewDirection = normalize(vec3(invV * vec4(0.0, 0.0, 0.0, 1.0) - position));

    float dotNL = dot(normalDirection, lightDirection);
    vec3 sample_color = texture(textureSampler, UV).rgb;
    
    color = (ambient + scene_ambient) * sample_color;
    if(dotNL > 0.0)
    {
        vec3 diffuseReflection = diffuse * dotNL * sample_color;
        vec3 specularReflection = specular * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), shininess);
        color += lightIntensity * (diffuseReflection + specularReflection);
    }
}
