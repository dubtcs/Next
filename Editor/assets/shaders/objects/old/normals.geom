#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projectionViewMatrix;
};

uniform mat4 projectionMatrix;

in vsOutput{
    vec3 Normal;
} vsInput[];

float mag = 0.5;

void CreateLine(int index)
{
    gl_Position = projectionMatrix * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projectionMatrix * (gl_in[index].gl_Position + vec4(vsInput[index].Normal, 0) * mag);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    CreateLine(0);
    CreateLine(1);
    CreateLine(2);
}
