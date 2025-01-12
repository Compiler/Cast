#version 460 core

layout (location = 2) in vec3 oNormal;
layout (location = 3) in vec3 oFragPos;

layout (location = 0) out vec4 FragColor;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

void main(){

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;

    vec3 norm = normalize(oNormal);
    vec3 lightDir = normalize(u_lightPos - oFragPos);

    float angle = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = angle * u_lightColor;

    float specularStrength = 0.5f;
    vec3 viewDir = normalize(u_viewPos - oFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * u_lightColor;

    //float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = diff * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    FragColor = vec4(result, 1.0);

}
