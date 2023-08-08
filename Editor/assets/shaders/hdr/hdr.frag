#version 460

out vec4 outColor;

uniform sampler2D ts[2];

in vec2 texturePosition;

const float gamma = 2.2;
const float exposure = 0.5;

void main()
{
    vec3 hdr = texture(ts[0], texturePosition).rgb;
    vec3 bloom = texture(ts[1], texturePosition).rgb;
    hdr += bloom;
    vec3 target = vec3(1.0) - exp(-hdr * exposure);
    target = pow(target, vec3(1.0 / gamma));
    outColor = vec4(target, 1.0);
}
