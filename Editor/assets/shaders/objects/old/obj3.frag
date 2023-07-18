#version 460 core

out vec4 outColor;

in vec2 pTexPos;
in vec3 pWorldPos;
in vec3 pNormal;

uniform sampler2D simpleTexture;
uniform samplerCube shadowMap;
uniform mat4 normalMatrix;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform float tiling;

uniform bool useBlinn;
uniform float farPlane;

float ambientStrength = 0.1;
float specularStrength = 0.4;
vec3 lightColor = vec3(1.0);

float shine = 64.0;
float reflectionStrength = 0.2;

// can do lighting in view space to remove lookvector variable

float GetShadowAmount(vec3 position)
{
    vec3 lightDirection2 = position - lightPosition;
    float closest = texture(shadowMap, lightDirection2).r;
    closest *= farPlane;
    float current = length(lightDirection2);
    float bias = 0.05;
    return (current - bias > closest ? 1.0 : 0.0);
}

void main()
{

    vec3 viewDirection = normalize(cameraPosition - pWorldPos);
    vec3 normal = normalize(pNormal);

    vec4 targetColor = texture(simpleTexture, pTexPos * tiling);

    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // Light Sources
    vec3 lightDirection = normalize(lightPosition - pWorldPos);
    vec3 halfwayNormal = normalize(lightDirection + viewDirection);

    // Diffuse
    float difference = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = difference * lightColor;

    // Specular
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularAmount = 0.0;

    if(useBlinn == true)
    {
        shine *= 2.0;
        specularAmount = pow(max(dot(normal, halfwayNormal), 0.0), shine);
    } 
    else
    {
        specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), shine);
    }
    vec3 specular = specularStrength * specularAmount * lightColor;

    float attenuation = 1 / (length(viewDirection));

    float shadowMask = GetShadowAmount(pWorldPos);

    vec4 final = (vec4((ambient + (1.0 - shadowMask) + diffuse + specular), 1.0) * targetColor * attenuation);
    outColor = final;
}
