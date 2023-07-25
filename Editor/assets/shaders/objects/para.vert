
#version 460

#define MAX_LIGHTS 10

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec4 vTangent;
layout (location = 3) in vec2 vTexturePosition;

struct Light
{
    vec3    Position;       //  0 offset, 16 align
    float   Intensity;      // 12 offset,  4 align
    vec3    Direction;      // 16 offset, 16 align
    uint    Type;           // 28 offset,  4 align
    vec3    Color;          // 32 offset, 16 align
    float   Radius;         // 44 offset,  4 align
};

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
    vec4 baseColor;         // 0  Offset
    int colorTextureIndex;  // 16 Offset
    float roughness;        // 20 Offset
    float metallic;         // 24 Offset
    int normalTexture;      // 28 Offset
    int emissionTexture;    // 32 Offset
    int occlusionTexture;   // 36 Offset
};

// Output
out vec3 pWorldPosition;
out vec3 pNormal;
out vec2 pTexPos;

out vec3 tangentCameraPosition;
out vec3 tangentWorldPosition;
out vec3 tangentLightPositions[MAX_LIGHTS];

void main()
{
    gl_Position = projectionViewMatrix * worldMatrix * vec4(vPosition, 1.0);
    pWorldPosition = vec3(worldMatrix * vec4(vPosition, 1.0));
    pNormal = mat3(normalMatrix) * vNormal;
    pTexPos = vTexturePosition;

    vec3 t = normalize(vec3(normalMatrix * vec4(vTangent.xyz, 0.0)));
    vec3 n = normalize(vec3(normalMatrix * vec4(vNormal, 0.0)));
    t = normalize(t - dot(t, n) * n);
    vec3 b = cross(n, t) * vTangent.w;

    mat3 tmat = mat3(t, b, n);
    mat3 tbn = transpose(tmat);

    tangentCameraPosition = tbn * cameraPosition;
    tangentWorldPosition = tbn * pWorldPosition;
    for(uint i = 0; i < lightsUsed; i++)
    {
        tangentLightPositions[i] = tbn * lights[i].Position;
    }

}

/*

vec3 t = normalize(pNormal);
    vec3 n = normalize(mat3(normalMatrix) * vTangent.xyz);

    vec3 b = normalize(cross(n, t)) * vTangent.w;

    pTangentMatrix = mat3(t, b, n);

    */
