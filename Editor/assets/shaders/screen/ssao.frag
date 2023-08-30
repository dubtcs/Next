#version 460

#define MAX_LIGHTS 10
#define CHECKBIT(var, index) ((var >> index) & 1U) == 1

#define LIGHT_POINT 0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_SPOT 2
#define LIGHT_AMBIENT 3

#define KERNEL_SIZE 64

/*
This caused me a lot of frustration.
I was comparing depth buffer values to view space Z values among other things.
View space normal matrix was wrong, fixed that. I was removing translation.
*/

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

vec3 GetWorldPosition(vec2 texpos, float depth)
{
    float z = (depth * 2.0) - 1.0;

    vec2 coordsClip = (texpos * 2.0) - 1.0;
    vec4 clipPosition = vec4(coordsClip, z, 1.0);
    
    vec4 viewPosition = projectionMatrixInverse * clipPosition;

    viewPosition.xyz /= viewPosition.w;
    vec4 worldPosition = viewMatrixInverse * viewPosition;

    return worldPosition.xyz;
}

float ExtractDepth(vec2 texpos)
{
    return texture(gTextures[0], texpos).r;
}

vec3 GetViewPosition(vec2 texpos, float depth)
{
    vec2 dpos = (texpos * 2.0) - 1.0;
    float dd = (depth * 2.0) - 1.0;

    vec4 devicePositions = vec4(dpos, dd, 1.0);
    vec4 viewPosition = projectionMatrixInverse * devicePositions;
    //viewPosition.z *= -1;

    return viewPosition.xyz / viewPosition.w;
}

vec3 GetViewNormal(vec2 texpos)
{
    vec4 normal = texture(gTextures[1], texpos);
    normal = normalViewMatrix * normal;
    return (normal.xyz);
}

const vec2 tiling = vec2(480.0, 270.0); // 1920/4, 1080/4 -- Adapt to resolution later

// this is driving me insane

const float radius = 0.5;
const float bias = 0.025;
const float aoIntensity = 2.5;

void main()
{
    // Positions have negative Z values

    vec3 viewPosition = GetViewPosition(texturePosition, ExtractDepth(texturePosition));
    vec3 viewNormal = GetViewNormal(texturePosition);
    vec3 noise = texture(gTextures[2], texturePosition * tiling).xyz;

    vec3 tangent = normalize(noise - viewNormal * dot(noise, viewNormal));
    vec3 bitangent = cross(viewNormal, noise);
    mat3 tbn = mat3(tangent, bitangent, viewNormal);

    float occlusion = 0.0;
    for (int i = 0; i < KERNEL_SIZE; i++)
    {
        vec3 samplePosition = tbn * kernel[i];
        samplePosition = viewPosition + (samplePosition * radius);

        vec4 offset = vec4(samplePosition, 1.0);
        offset = projectionMatrix * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5; // range [0, 1]
        
        float testDepth = ExtractDepth(offset.xy); // depth buffer value, range [0, 1]
        float testZ = GetViewPosition(offset.xy, testDepth).z; // actual viewspace Z value range [near_plane, far_plane]
        // REMEMBER, Z VALUES ARE NEGATIVE

        float rangeMod = smoothstep(0.0, 1.0, radius / abs(viewPosition.z - testZ));
        float addMod = (testZ >= samplePosition.z + bias) ? 1.0 : 0.0; // negative Z values. greater is closer to camera
        occlusion += addMod * rangeMod;
    }

    occlusion = 1 - (occlusion / KERNEL_SIZE);
    outColor = pow(occlusion, aoIntensity);
}
