#version 460

#define MAX_LIGHTS 10
#define BITCHECK(var, index) ((var >> index) & 1U) == 1

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
    vec3 cameraPosition;
};

const vec2 tiling = vec2(480, 270); // 1920/4, 1080/4 -- Adapt to resolution later

void main()
{
    vec3 worldPosition = texture(gTextures[0], texturePosition).xyz;
    vec3 normal = texture(gTextures[1], texturePosition).rgb;
    vec3 noise = texture(gTextures[2], texturePosition * tiling).xyz;

    // DAMNNNN
    // Normal already has TBN applied by vertex shader if it has tangents
    vec3 T = normalize(noise - normal * dot(noise, normal));
    vec3 B = cross(normal, T);
    mat3 TBN = mat3(T, B, normal);

    float occlusion = 0.0;
    const float radius = 0.5;
    for(int i = 0; i < KERNEL_SIZE; i++)
    {
        vec3 samplep = TBN * kernel[i];
        samplep = worldPosition + (samplep * radius);

        vec4 offset = vec4(samplep, 1.0);
        offset = projectionMatrix * offset;
        offset.xyz /= offset.w;
        offset.xyz = (offset.xyz * 0.5) + 0.5;
        
        float depth = texture(gTextures[0], offset.xy).z;

        float range = smoothstep(0.0, 1.0, radius / abs(worldPosition.z - depth));
        occlusion += (depth >= samplep.z + (0.25) ? 1.0 : 0.0) * range;  // 0.25 is bias, magic number
    }

    outColor = 1.0 - (occlusion / KERNEL_SIZE);

}
