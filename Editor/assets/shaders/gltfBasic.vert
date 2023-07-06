#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexPos;

uniform mat4 worldMatrix;

out vec2 pTexPos;

void main()
{
    gl_Position = worldMatrix * vec4(vPos, 1.0);
    pTexPos = vTexPos;
}
