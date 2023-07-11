#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexPos;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projectionViewMatrix;
};

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat4 worldMatrix;
uniform mat4 normalMatrix;

out vsOutput
{
    vec3 Normal;
} outblock;

void main()
{
    gl_Position = viewMatrix * worldMatrix * vec4(vPos, 1.0);
    outblock.Normal = mat3(normalMatrix) * vNormal;
}
