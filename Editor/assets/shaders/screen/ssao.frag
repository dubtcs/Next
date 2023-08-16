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
    mat4 viewMatrix;
    vec3 cameraPosition;
};

const vec2 tiling = vec2(480.0, 270.0); // 1920/4, 1080/4 -- Adapt to resolution later

// this is driving me insane

const float radius = 0.5;
const float bias = 0.025;

void main()
{

    vec3 worldPosition = texture(gTextures[0], texturePosition).xyz;
    vec3 position = (viewMatrix * vec4(worldPosition, 0.0)).xyz;

    vec3 worldNormal = texture(gTextures[1], texturePosition).xyz;
    vec3 normal = (viewMatrix * vec4(worldNormal, 0.0)).xyz;
    normal = normalize(normal);

    vec3 noise = texture(gTextures[2], texturePosition * tiling).xyz;

    vec3 tangent = normalize(noise - normal * dot(noise, normal));
    vec3 bitan = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitan, normal);

    float O = 0.0;
    for(int i = 0; i < KERNEL_SIZE; i++)
    {
        vec3 samplePosition = tbn * kernel[i];
        samplePosition = position + samplePosition * radius;

        vec4 offset = vec4(samplePosition, 1.0);
        offset = projectionMatrix * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        // offset already applied viewMatrix
        float sampleDepth = texture(gTextures[0], offset.xy).z;

        O += (sampleDepth >= (samplePosition.z + bias) ? 1.0 : 0.0);
    }

    outColor = O;

}

// void main()
// {
//     vec3 worldPosition = texture(gTextures[0], texturePosition).xyz;
//     vec3 position = (viewMatrix * vec4(worldPosition, 1.0)).xyz;
    
//     vec3 worldNormal = texture(gTextures[1], texturePosition).xyz;
//     vec3 normal = normalize((viewMatrix * vec4(worldNormal, 1.0)).xyz);

//     vec3 noise = normalize(texture(gTextures[2], texturePosition * tiling)).xyz;

//     vec3 tangent = normalize(noise - (normal * dot(noise, normal)));
//     vec3 bitangent = cross(normal, tangent);
//     mat3 TBN = mat3(tangent, bitangent, normal);

//     const float radius = 0.5;
//     const float bias = 0.025;

//     float O = 0.0;
//     for(int i = 0; i < KERNEL_SIZE; i++)
//     {
//         vec3 samplePosition = TBN * kernel[i];
//         samplePosition = position + samplePosition * radius;

//         vec4 offset = vec4(samplePosition, 1.0);
//         offset = projectionMatrix * offset;
//         offset.xyz /= offset.w;
//         offset.xyz = offset.xyz * 0.5 + 0.5;

//         vec3 worldOffset = texture(gTextures[0], offset.xy).xyz;
//         float sampleDepth = (viewMatrix * vec4(worldOffset, 1.0)).z;
        
//         //float sampleDepth = (viewMatrix * texture(gTextures[0], offset.xy)).z;
//         float rangeCheck = smoothstep(0.0, 1.0, radius / abs(position.z - sampleDepth));

//         O += (sampleDepth >= samplePosition.z + bias ? 1.0 : 0.0) * rangeCheck;
//         //O = sampleDepth;
//     }
//     O = 1.0 - (O / KERNEL_SIZE);
//     outColor = O;
    
// }

// void main()
// {
//     // Position and normal are in WORLD SPACE
//     //vec3 worldPosition = texture(gTextures[0], texturePosition).xyz;
//     //vec3 normal = normalize(texture(gTextures[1], texturePosition).xyz);

//     vec3 worldPosition = (viewMatrix * texture(gTextures[0], texturePosition)).xyz;
//     vec3 normal = (viewMatrix * texture(gTextures[1], texturePosition)).xyz;

//     vec2 tiledPosition = texturePosition * tiling;
//     vec3 noise = normalize(texture(gTextures[2], tiledPosition).xyz);

//     // Need to move the kernel vector to the space of the normal
//     // TBN matrix is just that
//     // Gram Schmidt
//     vec3 tangent = normalize(noise - (normal * dot(noise, normal))); // Projection of noise onto the normal 
//     vec3 bitangent = cross(normal, tangent);
//     mat3 TBN = mat3(tangent, bitangent, normal);

//     const float radius = 0.5;
//     const float bias = 0.025;

//     float occ = 0.0;
//     for(int i = 0; i < KERNEL_SIZE; i++)
//     {
//         vec3 kernelPosition = TBN * kernel[i];
//         kernelPosition = worldPosition + (kernelPosition * radius);

//         vec4 offset = vec4(kernelPosition, 1.0);
//         offset = projectionMatrix * offset;
//         offset.xyz /= offset.w;
//         offset.xyz = offset.xyz * 0.5 + 0.5;
        
//         float kernelDepth = (viewMatrix * texture(gTextures[0], offset.xy)).z;
//         float rangeMod = smoothstep(0.0, 1.0, radius / abs(worldPosition.z - kernelDepth));
        
//         float does = (kernelDepth >= kernelPosition.z + bias) ? 1.0 : 0.0;
//         occ += does * rangeMod;
//     }
//     occ = 1 - (occ / KERNEL_SIZE);
//     outColor = occ;

//     // float occlusion = 0.0;
//     // for(int i = 0; i < KERNEL_SIZE; i++)
//     // {
//     //     // moving it from tangent space at origin to view space around the normal
//     //     vec3 position = TBN * kernel[i];
//     //     position = worldPosition + (position * radius);

//     //     vec4 offset = vec4(position, 1.0);
//     //     offset = projectionMatrix * offset;
//     //     offset.xyz /= offset.w;
//     //     offset.xyz = offset.xyz * 0.5 + 0.5; // now in rang eof [0.0, 1.0]

//     //     // Z is taken bc it's in clip space now, and opengl uses [-1.0, 1.0]
//     //     float depth = texture(gTextures[0], offset.xy).z;
//     //     float rangeModifier = smoothstep(0.0, 1.0, radius / abs(worldPosition.z - depth));
//     //     occlusion += ((depth >= (position.z + bias)) ? 1.0 : 0.0);// * rangeModifier;
//     //     occlusion = depth;
//     // }

//     // occlusion = occlusion / KERNEL_SIZE;
//     // outColor = occlusion;

// }
