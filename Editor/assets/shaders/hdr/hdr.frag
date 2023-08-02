#version 460

layout (location = 0) out vec4 outColor;

in vec2 pPosition;

uniform sampler2D hdr;

//uniform sampler2DMS hdr;
//uniform int samples;
// Use these and texelFetch() to render ms textures

const float g = 2.2;
const float exposure = 0.5;

void main()
{
    vec3 target = texture(hdr, pPosition).rgb;
    vec3 mappedColor = vec3(1.0) - exp(-target * exposure);
    mappedColor = pow(mappedColor, vec3(1.0 / g));
    outColor = vec4(mappedColor, 1.0);
}
