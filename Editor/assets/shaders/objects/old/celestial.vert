#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexPos;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projectionViewMatrix;
    mat4 modelMatrices[100];
    mat4 normalMatrices[100];
};

out vec2 pTexPos;
out vec3 pWorldPos;
out vec3 pNormal;

void main()
{
    gl_Position = projectionViewMatrix * modelMatrices[gl_InstanceID] * vec4(vPos, 1.0);
    pTexPos = vTexPos;
    pNormal = mat3(normalMatrices[gl_InstanceID]) * vNormal;
    pWorldPos = vec3(modelMatrices[gl_InstanceID] * vec4(vPos, 1.0));
}
