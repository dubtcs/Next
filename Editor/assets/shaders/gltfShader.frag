#version 460 core

out vec4 outColor;

in vec2 pTexPos;

uniform sampler2D simpleTexture;

void main()
{
    outColor = texture(simpleTexture, pTexPos);
}
