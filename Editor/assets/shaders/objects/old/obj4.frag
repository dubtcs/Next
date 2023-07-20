
#version 460

in vec3 pWorldPosition;
in vec3 pNormal;
in vec2 pTexPos;
in vec4 pShadowSpace;

struct Light
{
    vec3    Position;       //  0 offset, 16 align
    float   Intensity;      // 12 offset,  4 align
    vec3    Direction;      // 16 offset, 16 align
    uint    Type;           // 28 offset,  4 align
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

uniform sampler2D shadowMap;

// Output
out vec4 outColor;

// Constants
vec4 targetColor = vec4(0.5, 0.25, 0.4, 1.0);
float ambientStrength = 0.0;
float specularStrength = 0.2;
float specularIntensity = 64;

float attenuationConstant = 1.0;
float attenuationLinear = 0.09;
float attenuationQuadratic = 0.032;

// Globals
vec3 normal = normalize(pNormal);

float GetAttenuation(uint index)
{
    float d = length(lights[index].Position - pWorldPosition);
    float at = 1.0 / (attenuationConstant + (attenuationLinear * d) + (attenuationQuadratic * (d * d)));
    return at;
}

float shadowBias = 0.005;
float GetShadow(uint index)
{
    vec3 projectionPosition = pShadowSpace.xyz / pShadowSpace.w;
    projectionPosition = projectionPosition * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projectionPosition.xy).r;
    return (projectionPosition.z - shadowBias > closestDepth) ? 1.0 : 0.0;
}

// Point
vec3 PointLight(uint index)
{
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 lightDirection = normalize(lights[index].Position - pWorldPosition);
    float angleDifference = max(dot(normal, lightDirection), 0.0);
    diffuse = angleDifference * lights[index].Color;

    vec3 viewDirection = normalize(cameraPosition - pWorldPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularHighlight = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularIntensity);
    specular = specularStrength * specularHighlight * lights[index].Color;

    return diffuse + specular;
}

// Directional
vec3 DirectionalLight(uint index)
{
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 lightDirection = normalize(-lights[index].Direction);
    float angleDifference = max(dot(normal, lightDirection), 0.0);
    diffuse = angleDifference * lights[index].Color;
    
    vec3 viewDirection = normalize(cameraPosition - pWorldPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularHighlight = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularIntensity);
    specular = specularStrength * specularHighlight * lights[index].Color;

    return (diffuse + specular) * (1.0 - GetShadow(index));
}

// Spot
vec3 SpotLight(uint index)
{
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 directionToLight = normalize(lights[index].Position - pWorldPosition);
    float angle = dot(directionToLight, normalize(-lights[index].Direction));

    if(angle > lights[index].Radius)
    {

        float falloffStart = lights[index].Radius - (lights[index].Radius * 0.1); // 10% feather
        float edge = (lights[index].Radius - falloffStart);
        float feather = clamp((angle - lights[index].Radius) / edge, 0.0, 1.0);

        float angleDifference = max(dot(normal, directionToLight), 0.0);
        diffuse = angleDifference * lights[index].Color * feather;

        vec3 lightDirection = normalize(-lights[index].Direction);
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        float specularHighlight = pow(max(dot(directionToLight, reflectionDirection), 0.0), specularIntensity);
        specular = specularStrength * specularHighlight * lights[index].Color * feather;
    }

    return diffuse + specular;
}

// Ambient
vec3 AmbientLight(uint index)
{
    vec3 color = vec3(lights[index].Color);
    return color;
}

void main()
{

    vec3 lightEffect = vec3(0.0);

    for(uint i = 0; i < lightsUsed; i++)
    {
        //float at = (lights[i].Type % 2 > 0) ? 1.0 : GetAttenuation(i);
        float at = GetAttenuation(i);
        float amnt = lights[i].Intensity;
        vec3 currentLightEffect = vec3(0.0);
        switch(lights[i].Type)
        {
            case(0): // Point
            {
                currentLightEffect = PointLight(i);
                break;
            }
            case(1): // Directional
            {
                currentLightEffect = DirectionalLight(i);
                at = 1.0;
                break;
            }
            case(2): // Spot
            {
                currentLightEffect = SpotLight(i);
                break;
            }
            case(3): // Ambient
            {
                currentLightEffect = AmbientLight(i);
                at = 1.0;
                break;
            }
        }
        lightEffect += (currentLightEffect * at * amnt);
    }

    vec4 currentTarget = vec4(lightEffect, 1.0) * targetColor;

    outColor = currentTarget;
}
