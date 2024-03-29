
#version 460

layout (location = 0) out vec4 outColor;

uniform int vert;
uniform sampler2D blurredImage;
in vec2 texturePosition;

//const float weights[5] = {0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216};
const float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 offset = 1.0 / textureSize(blurredImage, 0);
    vec3 target = texture(blurredImage, texturePosition).rgb * weights[0];
    if(vert > 0)
    {
        for (int i = 1; i < 5; i++)
        {
            target += texture(blurredImage, texturePosition + vec2(0.0, offset.y * i)).rgb * weights[i];
            target += texture(blurredImage, texturePosition - vec2(0.0, offset.y * i)).rgb * weights[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; i++)
        {
            target += texture(blurredImage, texturePosition + vec2(offset.x * i, 0.0)).rgb * weights[i];
            target += texture(blurredImage, texturePosition - vec2(offset.x * i, 0.0)).rgb * weights[i];
        }
    }
    
    outColor = vec4(target, 1.0);
    //outColor = vec4(0.6, 0.8, 0.6, 1.0);
}
