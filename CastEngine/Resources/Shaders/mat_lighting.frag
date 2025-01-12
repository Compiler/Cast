#version 460 core

layout (location = 2) in vec3 oNormal;
layout (location = 3) in vec3 oFragPos;

layout (location = 0) out vec4 FragColor;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material u_mat;

void main(){

    vec3 ambient = u_mat.ambient * u_lightColor;

    vec3 norm = normalize(oNormal);
    vec3 lightDir = normalize(u_lightPos - oFragPos);

    float angle = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = angle * u_lightColor * u_mat.diffuse;

    vec3 viewDir = normalize(u_viewPos - oFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_mat.shininess);
    vec3 specular = (spec * u_mat.specular) * u_lightColor;

    //float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = diff * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    FragColor = vec4(result, 1.0);

}
