#version 460 core

out vec4 outColor;

in vec2 pTexPos;

uniform sampler2D simpleTexture;

void main()
{
    outColor = vec4(0.5, 0.25, 0.9, 1.0);//texture(simpleTexture, pTexPos);
}
