
#version 460

in vec4 pixelPosition;

uniform vec3 lightPosition;
uniform float farPlane;

void main()
{
    // empty for GL to find depth?
    float lightDistance = length(pixelPosition.xyz - lightPosition);
    lightDistance = lightDistance / farPlane;
    gl_FragDepth = lightDistance;
}
