#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal; // unused
layout (location = 2) in vec2 vTexPos; // unused

// Uniform Buffers
layout (std140, binding = 0) uniform FrameInfo
{
    mat4 projectionViewMatrix;
    mat4 projectionMatrix;
    mat4 projectionMatrixInverse;
    mat4 viewMatrix;
    mat4 viewMatrixInverse;
    mat4 normalViewMatrix; // transpose(inverse(viewMatrix))
    mat4 projectionViewMatrixNoTranslation;
    vec3 cameraPosition;
    int xResolution;
    int yResolution;
};

out vec3 pTexPos;

void main()
{
    vec4 targetPosition = projectionViewMatrixNoTranslation * vec4(vPos, 1.0);
    gl_Position = targetPosition.xyww;
    pTexPos = vPos;
}
