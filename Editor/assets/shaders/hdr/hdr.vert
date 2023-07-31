#version 460

layout (location = 0) in vec3 vPosition;

out vec2 pPosition;

void main()
{
    gl_Position = vec4(vPosition, 1.0);
    pPosition = ((vPosition + 1) / 2).xy;
}
