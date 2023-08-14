#version 460

#define MAX_LIGHTS 10
#define BITCHECK(var, index) ((var >> index) & 1U) == 1

#define LIGHT_POINT 0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_SPOT 2
#define LIGHT_AMBIENT 3

layout (location = 0) out vec4 outColor;

// Keeping this 3. We don't need the colors texture.
// Texture units should be 0, 1, 4
uniform sampler2D gTextures[3];

uniform vec3 kernel[64];

in vec2 texturePosition;

void main()
{
    outColor = vec4(1.0);
}
