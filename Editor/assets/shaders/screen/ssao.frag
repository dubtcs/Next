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

const vec2 tiling = vec2(480.0, 270.0); // 1920/4, 1080/4 -- Adapt to resolution later

// this is driving me insane

const float radius = 0.5;
const float bias = 0.025;

void main()
{
    float depth = texture(gTextures[0], texturePosition).x;
    outColor = WorldPositionFromDepthValue(depth).x;
}
