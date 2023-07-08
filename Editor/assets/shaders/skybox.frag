#version 460 core

out vec4 outColor;

in vec3 pTexPos;

uniform samplerCube skybox;

void main()
{
    
    outColor = texture(skybox, pTexPos);

}
