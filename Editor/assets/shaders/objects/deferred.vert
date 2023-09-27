#version 460
#define MAX_LIGHTS 10

#define CHECKBIT(var, index) ((var >> index) & 1U) == 1
#define PRIM_BIT_NORMALS 0
#define PRIM_BIT_TANGENTS 1

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec4 vTangent;
layout (location = 3) in vec4 vColor;
layout (location = 4) in vec2 vTexturePosition;

out vec3 pWorldPosition;
out vec3 pNormal;
out vec4 pColor;
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
    mat4 projectionMatrixInverse;
    mat4 viewMatrix;
    mat4 viewMatrixInverse;
    mat4 normalViewMatrix; // transpose(inverse(viewMatrix))
    vec3 cameraPosition;
    int xResolution;
    int yResolution;
};
layout (std140, binding = 1) uniform LightInfo // Maybe switch to Shader Storage Buffer Object
{
    Light lights[MAX_LIGHTS];
    uint lightsUsed;    // 480 Offset
};
layout (std140, binding = 2) uniform ObjectInfo
{
    mat4 normalMatrix;  // 0    Offset // DEPRECATED BY PRIMITIVE INFO, USE primitiveMatrix
    mat4 worldMatrix;   // 64   Offset // DEPRECATED BY PRIMITIVE INFO, USE primitiveNormalMatrix
    int shaderMask;     // 128  Offset, bit 1: useTextures, bit 2: useLighting
};
layout (std140, binding = 3) uniform PrimitiveInfo
{
    mat4 primitiveMatrix;       // 0    Offset
    mat4 primitiveNormalMatrix; // 64   Offset
    vec4 baseColor;             // 128  Offset
    int colorTextureIndex;      // 144  Offset
    float roughness;            // 148  Offset
    float metallic;             // 152  Offset
    int normalTexture;          // 156   Offset
    int emissionTexture;        // 160   Offset
    int occlusionTexture;       // 164  Offset
    int primitiveMask;          // 168  Offset
};
layout (std140, binding = 4) uniform MorphInfo
{
    vec3 positionMorph;
    vec3 normalMorph;
    vec3 tangentMorph;
    vec2 texturePositionMorph;
    int morphMask; // bits [0, 3] 
};

/*
Primitive Mask Bits:
0 - Has Normals
1 - Has Tangents
*/

void main()
{
    //pWorldPosition = vec3(worldMatrix * vec4(vPosition, 1.0));
    vec3 morphPosition = vPosition;
    if(CHEKBIT(morphMask, 0))
    {
        morphPosition += positionMorph;
    }
    pWorldPosition = vec3(viewMatrix * primitiveMatrix * vec4(morphPosition, 1.0));
    
    pNormal = mat3(primitiveNormalMatrix) * vNormal;
    pTexPos = vTexturePosition;
    pColor = vColor;
    
    gl_Position = projectionMatrix * vec4(pWorldPosition, 1.0);

    if(CHECKBIT(primitiveMask, PRIM_BIT_TANGENTS))
    {
        vec3 t = normalize(vec3(primitiveMatrix * vec4(vTangent.xyz, 0.0)));
        vec3 b = cross(vNormal, vTangent.xyz) * vTangent.w;
        b = normalize(vec3(primitiveMatrix * vec4(b, 0.0)));
        vec3 n = normalize(vec3(primitiveNormalMatrix * vec4(vNormal, 0.0)));
        pTBN = mat3(t,b,n);
    }
    
}
