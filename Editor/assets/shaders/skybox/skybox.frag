#version 460 core

layout (location = 2) out vec4 gColor;

in vec3 pTexPos;

uniform samplerCube skybox;

void main()
{
    
    gColor = texture(skybox, pTexPos);

}
