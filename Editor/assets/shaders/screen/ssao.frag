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
    mat4 normalViewMatrix; // transpose(inverse(viewMatrix))
    vec3 cameraPosition;
};

const vec2 tiling = vec2(480.0, 270.0); // 1920/4, 1080/4 -- Adapt to resolution later

// this is driving me insane

const float radius = 0.5;
const float bias = 0.025;

// I can't put to words how much this has ruined my week/month/year and destroyed any and all confidence I had in myself whatsoever
// I should put my brain onto my ceiling

/*
  Positions and normals are stored in world space.
  Apply the view matrix to the position to get view space.
  Apply the normalViewMatrix to normals to get view space normals.
  The GBuffer normals already have the normal matrix applied to them from their vertex pass.

  T(I(view * world)) == T(I(view)) * normal;
  T(I(view * world)) == T(I(view)) * T(I(world));

  // I split the steps. Same thing, different look
*/

// fk it world space
void main()
{
    vec3 worldPosition = vec3(texture(gTextures[0], texturePosition));
    vec3 worldNormal = vec3(texture(gTextures[1], texturePosition));

    vec3 noise = vec3(texture(gTextures[2], texturePosition * tiling));

    vec3 tangent = normalize(noise - worldNormal * dot(noise, worldNormal));
    vec3 bitangent = cross(worldNormal, tangent);
    mat3 tbn = mat3(tangent, bitangent, worldNormal);

    float o = 0.0;
    for(int i = 0; i < KERNEL_SIZE; i++)
    {
        vec3 samplePosition = inverse(tbn) * kernel[i];
        samplePosition = worldPosition + samplePosition * radius;

        vec4 offset = vec4(samplePosition, 1.0);
        offset = projectionViewMatrix * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        float sampleDepth = texture(gTextures[0], offset.xy).z;

        float check = abs(worldPosition.z - sampleDepth) < radius ? 1.0 : 0.0;

        o += ((sampleDepth <= samplePosition.z) ? 1.0 : 0.0) * check;
    }

    o = 1 - (o / KERNEL_SIZE);
    outColor = o;
}

// void main()
// {
//     vec3 worldPosition = vec3(texture(gTextures[0], texturePosition));
//     vec3 viewPosition = mat3(viewMatrix) * worldPosition;

//     vec3 worldNormal = vec3(texture(gTextures[1], texturePosition));
//     vec3 viewNormal = mat3(normalViewMatrix) * worldNormal;
    
//     vec3 noise = vec3(texture(gTextures[2], texturePosition * tiling));

//     vec3 tangent = normalize(noise - (viewNormal * dot(noise, viewNormal)));
//     vec3 bitangent = cross(viewNormal, tangent);
//     mat3 tbn = mat3(tangent, bitangent, viewNormal);

//     float o = 0.0;

//     for(int i = 0; i < KERNEL_SIZE; i++)
//     {
//         //vec3 samplePosition = tbn * kernel[i];
//         //samplePosition = viewPosition + (samplePosition * radius); // moving it to the fragment location and blooming it a bit
//         //samplePosition = mat3(viewMatrix) * samplePosition;

//         vec3 samplePosition = tbn * kernel[i];
//         samplePosition = worldPosition + (samplePosition * radius);
//         samplePosition = (viewMatrix * vec4(samplePosition, 1.0)).xyz;
        
//         vec4 offset = vec4(samplePosition, 1.0);
//         offset = projectionMatrix * offset;     // to screen space/projection space
//         offset.xyz /= offset.w;                 // devide by perspective;
//         offset.xyz  = offset.xyz * 0.5 + 0.5;    // to device normalized coordinates;

//         // THIS IS A WORLD SPACE TEXTURE
//         vec4 worldSampleDepth = texture(gTextures[0], texturePosition);
//         float sampleDepth = (viewMatrix * worldSampleDepth).z; // convert world space coordinate into view space

//         float rangeCheck = smoothstep(0.0, 1.0, radius / abs(viewPosition.z - sampleDepth));
//         float modifier = (sampleDepth >= (samplePosition.z + bias)) ? 1.0 : 0.0;
//         modifier *= rangeCheck;

//         o += modifier;
//     }

//     outColor = o;
//     //outColor = viewNormal.x;
// }


// void main()
// {
//     vec3 worldPosition = vec3(texture(gTextures[0], texturePosition));
//     //vec3 position = vec3(viewMatrix * vec4(worldPosition, 0.0));
//     vec3 position = mat3(viewMatrix) * worldPosition;

//     vec3 worldNormal = vec3(texture(gTextures[1], texturePosition));
//     vec3 normal = mat3(normalViewMatrix) * worldNormal;

//     vec3 noise = vec3(texture(gTextures[2], texturePosition * tiling));

//     // These vectors are all in view space now
//     vec3 t = normalize(noise - normal * dot(noise, normal));
//     vec3 b = cross(normal, t);
//     mat3 tbn = mat3(t,b,normal);

//     float o = 0.0;
//     for (int i = 0; i < KERNEL_SIZE; i++)
//     {
//         vec3 samplePosition = tbn * kernel[i];
//         samplePosition = position + samplePosition * radius;

//         // view space
//         vec4 offset = vec4(samplePosition, 1.0);
//         offset = projectionMatrix * offset; // screen projection
//         offset.xyz /= offset.w;
//         offset.xyz = offset.xyz * 0.5 + 0.5;

//         // NEED to convert this into view space before sampling the depth.
//         // Texture is in world space, as said before
//         float sampleDepth = (viewMatrix * texture(gTextures[0], offset.xy)).z;

//         float rangeCheck = smoothstep(0.0, 1.0, radius / abs(position.z - sampleDepth));
        
//         o += (sampleDepth >= samplePosition.z + bias ? 1.0 : 0.0) * rangeCheck;
//     }
//     o = 1.0 - (o / KERNEL_SIZE);
    
//     outColor = o;
// }

// void main()
// {

//     vec3 worldPosition = texture(gTextures[0], texturePosition).xyz;
//     vec3 position = (viewMatrix * vec4(worldPosition, 0.0)).xyz;

//     vec3 worldNormal = texture(gTextures[1], texturePosition).xyz;
//     vec3 normal = (normalViewMatrix * vec4(worldNormal, 0.0)).xyz;
//     normal = normalize(normal);

//     vec3 noise = texture(gTextures[2], texturePosition * tiling).xyz;

//     vec3 tangent = normalize(noise - normal * dot(noise, normal));
//     vec3 bitan = cross(normal, tangent);
//     mat3 tbn = mat3(tangent, bitan, normal);

//     float O = 0.0;
//     for(int i = 0; i < KERNEL_SIZE; i++)
//     {
//         vec3 samplePosition = tbn * kernel[i];
//         samplePosition = position + samplePosition * radius;

//         vec4 offset = vec4(samplePosition, 1.0);
//         offset = projectionMatrix * offset;
//         offset.xyz /= offset.w;
//         offset.xyz = offset.xyz * 0.5 + 0.5;

//         // offset already applied viewMatrix
//         float sampleDepth = texture(gTextures[0], offset.xy).z;

//         O += (sampleDepth >= (samplePosition.z + bias) ? 1.0 : 0.0);
//     }
//     O = (O / KERNEL_SIZE); // 1 - () once it's done
//     outColor = O;

// }

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
