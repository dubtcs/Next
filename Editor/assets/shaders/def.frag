#version 460

#define MAX_LIGHTS 10
#define BITCHECK(var, index) ((var >> index) & 1U) == 1

#define LIGHT_POINT 0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_SPOT 2
#define LIGHT_AMBIENT 3

layout (location = 0) out vec4 outColor;

uniform sampler2D gTextures[3];

in vec2 texturePosition;

struct Light
{
    vec3    Position;       //  0 offset, 16 align
    float   Intensity;      // 12 offset,  4 align
    vec3    Direction;      // 16 offset, 16 align
    uint    Type;           // 28 offset,  4 align
    vec3    Color;          // 32 offset, 16 align
    float   Radius;         // 44 offset,  4 align
};

// Uniform Buffers
layout (std140, binding = 0) uniform FrameInfo
{
    mat4 projectionViewMatrix;
    vec3 cameraPosition;
    //float frameTime;
};
layout (std140, binding = 1) uniform LightInfo // Maybe switch to Shader Storage Buffer Object
{
    Light lights[MAX_LIGHTS];
    uint lightsUsed;    // 480 Offset
};

// LIGHTING
vec3 worldPosition = vec3(0.0);
vec3 normal = vec3(0.0);

float diffuseDampening = 1.0;
float specularDampening = 0.2;

float specularIntensity = 64;

float attenuationConstant = 1.0;
float attenuationLinear = 0.09;
float attenuationQuadratic = 0.032;

// LIGHTS
vec3 PointLight(uint i)
{
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 lightDirection = normalize(lights[i].Position - worldPosition);
    vec3 viewDirection = normalize(cameraPosition - worldPosition);

    float angleDifference = max(dot(normal, lightDirection), 0.0);
    diffuse = angleDifference * lights[i].Color;

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
    
    vec3 viewDirection = normalize(cameraPosition - worldPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularHighlight = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularIntensity);
    specular = specularHighlight * lights[i].Color * specularDampening;

    return (diffuse + specular);// * (1.0 - GetShadow(index));
}
vec3 SpotLight(uint i)
{
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 directionToLight = normalize(lights[i].Position - worldPosition);
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

float GetAttenuation(uint i)
{
    float d = length(lights[i].Position - worldPosition);
    float at = 1.0 / (attenuationConstant + (attenuationLinear * d) + (attenuationQuadratic * (d * d)));
    return at;
}

void main()
{
    worldPosition = texture(gTextures[0], texturePosition).rgb;
    normal = texture(gTextures[1], texturePosition).rgb;
    vec3 color = texture(gTextures[2], texturePosition).rgb;

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

    outColor = vec4(lightingEffect * color, 1.0);

}
