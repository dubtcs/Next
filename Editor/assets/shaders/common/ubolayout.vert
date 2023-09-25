
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
    mat4 normalMatrix;  // 0    Offset
    mat4 worldMatrix;   // 64   Offset
    int shaderMask;   // 128  Offset, bit 0: useTextures
};
layout (std140, binding = 3) uniform PrimitiveInfo
{
    mat4 primitiveMatrix;   // 0    Offset
    vec4 baseColor;         // 64   Offset
    int colorTextureIndex;  // 80   Offset
    float roughness;        // 84   Offset
    float metallic;         // 88   Offset
    int normalTexture;      // 92   Offset
    int emissionTexture;    // 96   Offset
    int occlusionTexture;   // 100  Offset
    int primitiveMask;      // 104  Offset
};
