#version 460

out vec4 outColor;

in vec2 fTexPos;

uniform sampler2D simpleTexture;

void main()
{
    outColor = texture(simpleTexture, fTexPos) * vec4(fTexPos, 1.0, 1.0);
    //vec4(fTexPos, 1.0, 1.0);
}
