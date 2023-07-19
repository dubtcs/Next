
#version 460

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

uniform mat4 projectionViewMatrix;
uniform mat4 worldMatrix;

void main()
{
    gl_Position = projectionViewMatrix * worldMatrix * vec4(vPos, 1.0);
}
