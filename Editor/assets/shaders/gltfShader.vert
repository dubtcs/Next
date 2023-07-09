#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexPos;

uniform mat4 worldMatrix;
uniform mat4 projectionViewMatrix;
uniform mat4 normalMatrix;

out vec2 pTexPos;
out vec3 pWorldPos;
out vec3 pNormal;

void main()
{
    gl_Position = projectionViewMatrix * worldMatrix * vec4(vPos, 1.0);
    pTexPos = vTexPos;
    pNormal = mat3(normalMatrix) * vNormal;
    pWorldPos = vec3(worldMatrix * vec4(vPos, 1.0));
}
