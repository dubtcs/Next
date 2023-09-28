#version 460
#define MAX_LIGHTS 10

#define CHECKBIT(var, index) ((var >> index) & 1U) == 1
#define PRIM_BIT_NORMALS 0
#define PRIM_BIT_TANGENTS 1

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

// Input
in vec3 pWorldPosition;
in vec3 pNormal;
in vec4 pColor;
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
    mat4 projectionMatrixInverse;
    mat4 viewMatrix;
    mat4 viewMatrixInverse;
    mat4 normalViewMatrix; // transpose(inverse(viewMatrix))
    mat4 projectionViewMatrixNoTranslation;
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
    mat4 normalMatrix;  // 0    Offset
    mat4 worldMatrix;   // 64   Offset
    int shaderMask;   // 128  Offset, bit 0: useTextures
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

/*
Primitive Mask Bits:
0 - Has Normals
1 - Has Tangents
*/

void main()
{
    gPosition = pWorldPosition;

    if(CHECKBIT(primitiveMask, PRIM_BIT_TANGENTS))
    {
        vec3 normal = texture(textures[normalTexture], pTexPos).rgb;
        normal = normalize((normal * 2.0) - 1.0);
        gNormal = normalize(pTBN * normal);
    }
    else
    {
        if(CHECKBIT(primitiveMask, PRIM_BIT_NORMALS))
        {
            gNormal = normalize(pNormal);
        } 
        else
        {
            gNormal = vec3(0.0);
        }
    }

    //vec4 targetColor = vec4(0.5, 0.07, 1.0, 1.0); // purple
    vec4 targetColor = baseColor * pColor;
    if(CHECKBIT(shaderMask, 0))
    {
        targetColor = texture(textures[colorTextureIndex], pTexPos) * targetColor;
    }
    gColor = targetColor;
}
