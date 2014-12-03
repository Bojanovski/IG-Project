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

uniform vec3 lightDirection = vec3(1.0, 0.0, 0.0);
uniform vec3 lightIntensity = vec3(1.0, 1.0, 1.0);

void main()
{
    vec3 normalDirection = normalize(varyingNormalDirection);
    vec3 viewDirection = normalize(vec3(invV * vec4(0.0, 0.0, 0.0, 1.0) - position));

    vec3 diffuseReflection = lightIntensity * diffuse * max(0.0, dot(normalDirection, lightDirection));

    vec3 specularReflection;
    if (dot(normalDirection, lightDirection) < 0.0) // light source on the wrong side?
    {
        specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
    }
    else // light source on the right side
    {
        specularReflection = lightIntensity * specular * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), shininess);
    }

    color = ambient + diffuseReflection + specularReflection;
}