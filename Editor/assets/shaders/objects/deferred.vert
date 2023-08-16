#version 460
#define MAX_LIGHTS 10

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec4 vTangent;
layout (location = 3) in vec2 vTexturePosition;

out vec3 pWorldPosition;
out vec3 pNormal;
out vec2 pTexPos;
out mat3 pTBN;

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
    pWorldPosition = vec3(worldMatrix * vec4(vPosition, 1.0));
    gl_Position = projectionViewMatrix * vec4(pWorldPosition, 1.0);
    pNormal = mat3(normalMatrix) * vNormal;
    pTexPos = vTexturePosition;

    if(hasTangents)
    {
        vec3 t = normalize(vec3(worldMatrix * vec4(vTangent.xyz, 0.0)));
        vec3 b = cross(vNormal, vTangent.xyz) * vTangent.w;
        b = normalize(vec3(worldMatrix * vec4(b, 0.0)));
        vec3 n = normalize(vec3(worldMatrix * vec4(vNormal, 0.0)));
        pTBN = mat3(t,b,n);
    }
    
}
