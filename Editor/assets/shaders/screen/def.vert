#version 460

layout (location = 0) in vec3 vPos;
layout (location = 3) in vec2 vTexturePosition;

out vec2 texturePosition;

void main()
{
    gl_Position = vec4(vPos, 1.0);
    texturePosition = vTexturePosition;
}
