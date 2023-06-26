#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexPos;

out vec2 pTexPos;

uniform mat4 worldMatrix;

void main()
{
    gl_Position = worldMatrix * vec4(vPos, 1.0);
    pTexPos = vTexPos;
}