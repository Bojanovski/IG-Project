#version 330 core

in vec4 position;  // position of the vertex (and fragment) in world space
in vec3 varyingNormalDirection;  // surface normal vector in world space
in vec2 UV;  // surface normal vector in world space

// Ouput data
out vec3 color;

uniform mat4 invV;
 
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform vec3 lightDirection;
uniform vec3 lightIntensity;
uniform sampler2D textureSampler;

void main()
{
    vec3 normalDirection = normalize(varyingNormalDirection);
    vec3 viewDirection = normalize(vec3(invV * vec4(0.0, 0.0, 0.0, 1.0) - position));

    float dotNL = dot(normalDirection, lightDirection);
    
    color = ambient;
    if(dotNL > 0.0)
    {
        vec3 diffuseReflection =  diffuse * dotNL * texture(textureSampler, UV).rgb;
        vec3 specularReflection = specular * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), shininess);
        color += lightIntensity * (diffuseReflection + specularReflection);
    }
}