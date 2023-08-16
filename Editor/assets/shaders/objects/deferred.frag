#version 460
#define MAX_LIGHTS 10

#define BITCHECK(var, index) ((var >> index) & 1U) == 1

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

// Input
in vec3 pWorldPosition;
in vec3 pNormal;
in vec2 pTexPos;
in mat3 pTBN;

uniform sampler2D textures[16];

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
    mat4 projectionMatrix;
    mat4 viewMatrix;
    vec3 cameraPosition;
};
layout (std140, binding = 1) uniform LightInfo // Maybe switch to Shader Storage Buffer Object
{
    Light lights[MAX_LIGHTS];
    uint lightsUsed;    // 480 Offset
};
layout (std140, binding = 2) uniform ObjectInfo
{
    mat4 normalMatrix;  // 0    Offset
    mat4 worldMatrix;   // 64   Offset
    int lightingMask;   // 128  Offset, bit 1: useTextures, bit 2: useLighting
};
layout (std140, binding = 3) uniform PrimitiveInfo
{
    vec4 baseColor;         // 0   Offset
    int colorTextureIndex;  // 16  Offset
    float roughness;        // 20  Offset
    float metallic;         // 24  Offset
    int normalTexture;      // 28 Offset
    int emissionTexture;    // 32 Offset
    int occlusionTexture;   // 36 Offset
    bool hasTangents;       // 40
    bool hasMaterial;       // 44
};

void main()
{
    gPosition = pWorldPosition;

    if(hasTangents)
    {
        vec3 normal = texture(textures[normalTexture], pTexPos).rgb;
        normal = normalize((normal * 2.0) - 1.0);
        gNormal = normalize(pTBN * normal);
    }
    else
    {
        gNormal = pNormal;
    }

    vec4 targetColor = vec4(0.5, 0.07, 1.0, 1.0); // purple
    if(BITCHECK(lightingMask, 1))
    {
        targetColor = texture(textures[colorTextureIndex], pTexPos) * baseColor;
    }
    gColor = targetColor;
}
