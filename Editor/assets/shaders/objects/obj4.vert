#version 460

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexPos;

layout (std140, binding = 0) uniform SceneInfo
{
    mat4 projectionViewMatrix;
    vec3 cameraPosition;
};

uniform mat4 worldMatrix;
uniform mat4 normalMatrix;

// Output
out vec3 pWorldPosition;
out vec3 pNormal;
out vec2 pTexPos;

// Main
void main()
{
    gl_Position = projectionViewMatrix * worldMatrix * vec4(vPos, 1.0);
    pWorldPosition = vec3(worldMatrix * vec4(vPos, 1.0));
    pNormal = mat3(normalMatrix) * vNormal;
    pTexPos = vTexPos;
}
