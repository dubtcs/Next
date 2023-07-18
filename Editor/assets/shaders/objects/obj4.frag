
#version 460

in vec3 pWorldPosition;
in vec3 pNormal;
in vec2 pTexPos;

struct Light
{
    vec3    Position;       //  0 offset, 16 align
    float   Intensity;      // 12 offset,  4 align
    vec3    Direction;      // 16 offset, 16 align
    int     LightType;      // 28 offset,  4 align
    vec3    Color;          // 32 offset, 16 align
    float   Radius;         // 44 offset,  4 align
};                          // 48 size,   16 align

#define MAX_LIGHTS 10
layout(std140, binding = 0) uniform SceneInfo
{
    mat4 projectionViewMatrix;
    vec3 cameraPosition;
};
layout (std140, binding = 1) uniform LightInfo
{
    Light lights[MAX_LIGHTS];
    uint lightsUsed;
};

// Output
out vec4 outColor;

// Constants
vec4 targetColor = vec4(0.5, 0.25, 0.4, 1.0);
float ambientStrength = 0.2;
float specularStrength = 0.2;
float specularIntensity = 64;

// Globals
vec3 normal = normalize(pNormal);

vec3 GetDiffuse(uint index)
{
    vec3 lightDirection = normalize(lights[index].Position - pWorldPosition);
    float angleDifference = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = angleDifference * lights[index].Color;
    return diffuse;
}

vec3 GetSpecular(uint index)
{
    vec3 lightDirection = normalize(lights[index].Position - pWorldPosition);
    vec3 viewDirection = normalize(cameraPosition - pWorldPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularHighlight = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularIntensity);
    return specularStrength * specularHighlight * lights[index].Color;
}

void main()
{

    vec3 diffuse;
    vec3 specular;
    
    for(uint i = 0; i < lightsUsed; i++)
    {
        diffuse += GetDiffuse(i);
        specular += GetSpecular(i);
    }

    vec4 currentTarget = vec4(ambientStrength * lights[0].Color + diffuse + specular, 1.0) * targetColor;

    outColor = currentTarget;
}
