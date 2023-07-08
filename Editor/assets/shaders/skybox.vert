#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal; // unused
layout (location = 2) in vec2 vTexPos; // unused

uniform mat4 projectionView;

out vec3 pTexPos;

void main()
{
    vec4 targetPosition = projectionView * vec4(vPos, 1.0);
    gl_Position = targetPosition.xyww;
    pTexPos = vPos;
}
