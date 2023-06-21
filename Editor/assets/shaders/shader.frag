#version 460 core

out vec4 outColor;

in vec2 fTexPos;

uniform sampler2D tex;

void main()
{
    outColor = texture(tex, fTexPos);
}
