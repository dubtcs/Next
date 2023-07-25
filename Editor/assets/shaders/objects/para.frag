#version 460

#define MAX_LIGHTS 10

// Input
in vec3 pWorldPosition;
in vec3 pNormal;
in vec2 pTexPos;

in vec3 tangentCameraPosition;
in vec3 tangentWorldPosition;
in vec3 tangentLightPositions[MAX_LIGHTS];

struct Light
{
    vec3    Position;       //  0 offset, 16 align
    float   Intensity;      // 12 offset,  4 align
    vec3    Direction;      // 16 offset, 16 align
    uint    Type;           // 28 offset,  4 align
    vec3    Color;          // 32 offset, 16 align
    float   Radius;         // 44 offset,  4 align
};

uniform sampler2D textures[16];
uniform bool useNormals;
uniform sampler2D depthTexture;

layout (std140, binding = 0) uniform FrameInfo
{
    mat4 projectionViewMatrix;
    vec3 cameraPosition;
    //float frameTime;
};
layout (std140, binding = 1) uniform LightInfo // Maybe switch to Shader Storage Buffer Object
{
    Light lights[MAX_LIGHTS];
    uint lightsUsed;
};
layout (std140, binding = 2) uniform ObjectInfo
{
    mat4 normalMatrix;  // 0    Offset
    mat4 worldMatrix;   // 64   Offset
};
layout (std140, binding = 3) uniform MaterialInfo
{
    vec4 baseColor;         // 0   Offset
    int colorTextureIndex;  // 16  Offset
    float roughness;        // 20  Offset
    float metallic;         // 24  Offset
    int normalTexture;      // 28 Offset
    int emissionTexture;    // 32 Offset
    int occlusionTexture;   // 36 Offset
};

// Output
out vec4 outColor;

// LIGHTING
vec3 normal = vec3(0.0);

float diffuseDampening = 1.0;
float specularDampening = 0.2;

float specularIntensity = 64;

float attenuationConstant = 1.0;
float attenuationLinear = 0.09;
float attenuationQuadratic = 0.032;

const float HEIGHT_MODIFIER = 0.1;

float GetAttenuation(uint i)
{
    float d = length(tangentLightPositions[i] - tangentWorldPosition);
    float at = 1.0 / (attenuationConstant + (attenuationLinear * d) + (attenuationQuadratic * (d * d)));
    return at;
}
vec3 PointLight(uint i)
{
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 lightDirection = normalize(tangentLightPositions[i] - tangentWorldPosition);
    float angleDifference = max(dot(normal, lightDirection), 0.0);
    diffuse = angleDifference * lights[i].Color;

    vec3 viewDirection = normalize(tangentCameraPosition - tangentWorldPosition);
    //vec3 reflectionDirection = reflect(-lightDirection, normal);
    //float specularHighlight = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularIntensity);
    vec3 halfwayNormal = normalize(lightDirection + viewDirection);
    float specularHighlight = pow(max(dot(normal, halfwayNormal), 0.0), specularIntensity);
    specular = specularHighlight * lights[i].Color * specularDampening;

    return diffuse + specular;
}
vec3 DirectionalLight(uint i)
{
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 lightDirection = normalize(-lights[i].Direction);
    float angleDifference = max(dot(normal, lightDirection), 0.0);
    diffuse = angleDifference * lights[i].Color;
    
    vec3 viewDirection = normalize(tangentCameraPosition - tangentWorldPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularHighlight = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularIntensity);
    specular = specularHighlight * lights[i].Color * specularDampening;

    return (diffuse + specular);// * (1.0 - GetShadow(index));
}
vec3 SpotLight(uint i)
{
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    //vec3 directionToLight = normalize(lights[i].Position - pWorldPosition);
    vec3 directionToLight = normalize(tangentLightPositions[i] - tangentWorldPosition);
    float angle = dot(directionToLight, normalize(-lights[i].Direction));

    if(angle > lights[i].Radius)
    {

        float falloffStart = lights[i].Radius - (lights[i].Radius * 0.1); // 10% feather
        float edge = (lights[i].Radius - falloffStart);
        float feather = clamp((angle - lights[i].Radius) / edge, 0.0, 1.0);

        float angleDifference = max(dot(normal, directionToLight), 0.0);
        diffuse = angleDifference * lights[i].Color * feather;

        vec3 lightDirection = normalize(-lights[i].Direction);
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        float specularHighlight = pow(max(dot(directionToLight, reflectionDirection), 0.0), specularIntensity);
        specular = specularHighlight * lights[i].Color * feather * specularDampening;
    }

    return diffuse + specular;
}
vec3 AmbientLight(uint i)
{
    vec3 color = vec3(lights[i].Color);
    return color;
}

vec2 ParallaxCoordinates()
{
    vec3 viewDirection = normalize(tangentCameraPosition - tangentWorldPosition);
    float height = texture(depthTexture, pTexPos).r;
    vec2 offset2 = viewDirection.xy / viewDirection.z * (height * HEIGHT_MODIFIER);
    return pTexPos - offset2;
}

void main()
{
    //vec2 texturePosition = ParallaxCoordinates();
    vec4 targetColor = texture(textures[colorTextureIndex], pTexPos) * baseColor;

    if(normalTexture >= 0 && useNormals == true)
    {
        normal = texture(textures[normalTexture], pTexPos).rgb;
        normal = normalize((normal * 2.0) - 1.0);
        //normal = normalize(pTangentMatrix * normal);
    }
    else
    {
        normal = normalize(pNormal);
    }

    vec3 lightingEffect = vec3(0.0);
    for(uint i = 0; i < lightsUsed; i++)
    {
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
        lightingEffect += (currentLightEffect * at * amnt);
    }

    outColor = vec4(lightingEffect, 1.0) * targetColor;
    //outColor = texture(textures[0], pTexPos);

}
