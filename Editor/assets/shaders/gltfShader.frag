#version 460 core

out vec4 outColor;

in vec2 pTexPos;
in vec3 pWorldPos;
in vec3 pNormal;

uniform sampler2D simpleTexture;
uniform mat4 normalMatrix;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

float ambientStrength = 0.1;
float specularStrength = 0.4;
vec3 lightColor = vec3(1.0);

float shine = 256.0;

// can do lighting in view space to remove lookvector variable

void main()
{
    

    vec4 targetColor = texture(simpleTexture, pTexPos);
    //vec4 targetColor = vec4(0.5, 0.2, 0.6, 1.0);
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(pNormal);
    vec3 lightDirection = normalize(lightPosition - pWorldPos);

    float difference = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = difference * lightColor;

    vec3 viewDirection = normalize(cameraPosition - pWorldPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), shine);
    vec3 specular = specularStrength * specularAmount * lightColor;

    vec4 final = vec4((ambient + diffuse + specular), 1.0) * targetColor;
    outColor = final;

}
