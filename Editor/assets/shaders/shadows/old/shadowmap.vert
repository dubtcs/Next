

#version 460

layout (location = 0) in vec3 vPos;

uniform mat4 modelMatrix;

void main()
{
    gl_Position = modelMatrix * vec4(vPos, 1.0);
}
