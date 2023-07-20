
#version 460

in vec4 pWorldPosition;

out vec4 outColor;

float farPlane = 25.0;

struct Light
{
    vec3    Position;       //  0 offset, 16 align
    float   Intensity;      // 12 offset,  4 align
    vec3    Direction;      // 16 offset, 16 align
    uint    Type;           // 28 offset,  4 align
    vec3    Color;          // 32 offset, 16 align
    float   Radius;         // 44 offset,  4 align
};                          // 48 size,   16 align

#define MAX_LIGHTS 10
layout (std140, binding = 1) uniform LightInfo
{
    Light lights[MAX_LIGHTS];
    uint lightsUsed;
};

void main()
{
    float d = length(pWorldPosition.xyz - lights[1].Position);
    d /= farPlane;
    gl_FragDepth = d;
}
