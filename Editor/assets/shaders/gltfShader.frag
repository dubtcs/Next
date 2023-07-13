#version 460 core

out vec4 outColor;

in vec2 pTexPos;
in vec3 pWorldPos;
in vec3 pNormal;

uniform sampler2D simpleTexture;
uniform samplerCube skybox;
uniform mat4 normalMatrix;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform bool useBlinn;

float ambientStrength = 0.1;
float specularStrength = 0.4;
vec3 lightColor = vec3(1.0);

float shine = 256.0;
float reflectionStrength = 0.2;

// can do lighting in view space to remove lookvector variable

void main()
{

    vec3 viewDirection = normalize(cameraPosition - pWorldPos);
    vec3 normal = normalize(pNormal);

    vec4 targetColor = texture(simpleTexture, pTexPos);

    // Reflection
    vec3 reflectionReflectVector = reflect(-viewDirection, normal);
    vec4 reflectionColor = vec4(texture(skybox, reflectionReflectVector).rgb, 1.0);
    reflectionColor *= reflectionStrength;

    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // Light Sources
    vec3 lightDirection = normalize(lightPosition - pWorldPos);
    vec3 halfwayNormal = normalize(lightDirection + viewDirection);

    // Diffuse
    float difference = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = difference * lightColor;

    // Specular
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularAmount = 0.0;

    if(useBlinn == true)
    {
        shine *= 2.0;
        specularAmount = pow(max(dot(normal, halfwayNormal), 0.0), shine);
    } 
    else
    {
        specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), shine);
    }
    vec3 specular = specularStrength * specularAmount * lightColor;

    vec4 final = (vec4((ambient + diffuse + specular), 1.0) * targetColor) + reflectionColor;
    outColor = final;

}
