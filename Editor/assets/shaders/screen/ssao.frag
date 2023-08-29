#version 460

#define MAX_LIGHTS 10
#define CHECKBIT(var, index) ((var >> index) & 1U) == 1

#define LIGHT_POINT 0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_SPOT 2
#define LIGHT_AMBIENT 3

#define KERNEL_SIZE 64

layout (location = 0) out float outColor;

// Keeping this 3. We don't need the colors texture.
// Texture units should be 0, 1, 3
uniform sampler2D gTextures[3];

uniform vec3 kernel[KERNEL_SIZE];

in vec2 texturePosition;

layout (std140, binding = 0) uniform FrameInfo
{
    mat4 projectionViewMatrix;
    mat4 projectionMatrix;
    mat4 projectionMatrixInverse;
    mat4 viewMatrix;
    mat4 viewMatrixInverse;
    mat4 normalViewMatrix; // transpose(inverse(viewMatrix))
    vec3 cameraPosition;
};

vec3 WorldPositionFromDepthValue(float depth)
{
    float z = (depth * 2.0) - 1.0;

    vec2 coordsClip = (texturePosition * 2.0) - 1.0;
    vec4 clipPosition = vec4(coordsClip, z, 1.0);
    
    vec4 viewPosition = projectionMatrixInverse * clipPosition;

    viewPosition.xyz /= viewPosition.w;
    vec4 worldPosition = viewMatrixInverse * viewPosition;

    return worldPosition.xyz;
}

float ExtractDepth()
{
    return texture(gTextures[0], texturePosition).r;
}

vec3 GetViewPosition_old()
{
    vec4 pos = vec4(texturePosition, texture(gTextures[0], texturePosition).x, 1.0);
    pos.z = 2.0 * pos.z - 1.0;
    pos = projectionMatrixInverse * pos;
    return pos.xyz / pos.w;
}

vec3 GetViewPosition()
{
    vec2 dpos = (texturePosition * 2.0) - 1.0;
    float dd = (ExtractDepth() * 2.0) - 1.0;

    vec4 devicePositions = vec4(dpos, dd, 1.0);
    vec4 viewPosition = projectionMatrixInverse * devicePositions;

    return viewPosition.xyz / viewPosition.w;
}

const vec2 tiling = vec2(480.0, 270.0); // 1920/4, 1080/4 -- Adapt to resolution later

// this is driving me insane

const float radius = 0.5;
const float bias = 0.025;

// vec4((viewMatrix * vec4(WorldPositionFromDepthValue(de), 1.0)).xyz, 1.0)

// this is gonna make be kill myself
void main()
{
    vec3 viewPosition = GetViewPosition();
    vec3 viewNormal = (normalViewMatrix * texture(gTextures[1], texturePosition)).xyz;
    vec3 random = texture(gTextures[2], texturePosition * tiling).xyz;

    vec3 tangent = normalize(random - viewNormal * dot(random, viewNormal));
    vec3 bitangent = cross(viewNormal, tangent);
    mat3 tbn = mat3(tangent, bitangent, viewNormal);

    float o = 0.0;
    for(int i = 0; i < KERNEL_SIZE; i++)
    {
        vec3 samplePosition = tbn * kernel[i];
        samplePosition = viewPosition + samplePosition * radius;

        vec4 offset = vec4(samplePosition, 1.0);
        offset = projectionMatrix * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = texture(gTextures[0], offset.xy).r;

        o += (sampleDepth >= samplePosition.z + bias) ? 1.0 : 0.0;
    }

    outColor = o;
}

// void main()
// {
//     float depth = ExtractDepth();

//     vec3 worldPosition = WorldPositionFromDepthValue(depth);
//     vec3 worldNormal = texture(gTextures[1], texturePosition).xyz;

//     vec3 viewPosition = (viewMatrix * vec4(worldPosition, 1.0)).xyz;
//     vec3 viewNormal = (normalViewMatrix * vec4(worldNormal, 1.0)).xyz;
    
//     vec3 randomVector = texture(gTextures[2], texturePosition).xyz;

//     vec3 tangent = normalize(randomVector - viewNormal * dot(randomVector, viewNormal));
//     vec3 bitangent = cross(viewNormal, tangent);
//     mat3 tbn = mat3(tangent, bitangent, viewNormal);

//     float o = 0.0;
//     for (int i = 0; i < KERNEL_SIZE; i++)
//     {
//         vec3 samplePosition = tbn * kernel[i];
//         samplePosition = viewPosition + samplePosition * radius;

//         vec4 offset = vec4(samplePosition, 1.0);
//         offset = projectionMatrix * offset;
//         offset.xyz /= offset.w;
//         offset.xyz = offset.xyz * 0.5 + 0.5;

//         float sampleDepth = texture(gTextures[0], offset.xy).r;

//         o += (sampleDepth >= (1.0 - samplePosition.z) + bias ? 1.0 : 0.0);
//     }

//     outColor = o;
// }

// void main()
// {
//     float pixelDepth = ExtractDepth();
//     vec3 worldPosition = WorldPositionFromDepthValue(pixelDepth);
//     vec3 worldNormal = texture(gTextures[1], texturePosition).xyz;

//     vec3 viewPosition = (viewMatrix * vec4(worldPosition, 0.0)).xyz;
//     vec3 viewNormal = (normalViewMatrix * vec4(worldNormal, 0.0)).xyz;

//     vec3 randomVector = texture(gTextures[2], texturePosition).xyz;

//     vec3 tangent = normalize(randomVector - (viewNormal * dot(randomVector, viewNormal)));
//     vec3 bitangent = normalize(cross(viewNormal, tangent));
//     mat3 tbn = mat3(tangent, bitangent, viewNormal);

//     float o = 0.0;

//     o = viewPosition.z;

//     outColor = o;
// }

// void main()
// {
//     float depth = texture(gTextures[0], texturePosition).x;
//     vec3 worldPosition = WorldPositionFromDepthValue(depth);
//     vec3 worldNormal = texture(gTextures[1], texturePosition).xyz;

//     vec3 viewPosition = (viewMatrix * vec4(worldPosition, 1.0)).xyz;
//     vec3 viewNormal = mat3(normalViewMatrix) * worldNormal * 2.0 - 1.0; // * 2.0 - 1.0;

//     vec3 noise = texture(gTextures[2], texturePosition * tiling).xyz;

//     vec3 tangent = normalize(noise - (viewNormal * dot(noise, viewNormal)));
//     vec3 bitangent = cross(viewNormal, tangent);
//     mat3 tbn = mat3(tangent, bitangent, viewNormal);

//     float o = 0.0;
//     for(int i = 0; i < KERNEL_SIZE; i++)
//     {
//         vec3 samplePosition = tbn * kernel[i];
//         samplePosition = (samplePosition * radius) + viewPosition;

//         vec4 offset = vec4(samplePosition, 1.0);
//         offset = projectionMatrix * offset;
//         offset.xy /= offset.w;
//         offset.xy = offset.xy * 0.5 + 0.5;

//         float sampleDepth = (viewMatrix * texture(gTextures[0], offset.xy)).x;
//         o += sampleDepth >= samplePosition.z + bias ? 1.0 : 0.0;
//     }

//     outColor = o;
// }
