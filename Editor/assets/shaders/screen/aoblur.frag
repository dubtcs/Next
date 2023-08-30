#version 460

#define MAX_LIGHTS 10
#define CHECKBIT(var, index) ((var >> index) & 1U) == 1

layout (location = 0) out float outColor;

in vec2 texturePosition;

uniform sampler2D aoTexture;

void main()
{
    vec2 size = 1.0 / vec2(textureSize(aoTexture, 0));
    float r = 0.0;
    for (int x = -2; x < 2; x++)
    {
        for (int y = -2; y < 2; y++)
        {
            vec2 offset = vec2(float(x), float(y)) * size;
            r += texture(aoTexture, texturePosition + offset).r;
        }
    }
    outColor = r / 16; // 16 is 4 * 4... texture dimension
}
