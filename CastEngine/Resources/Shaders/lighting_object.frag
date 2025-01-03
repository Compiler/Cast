#version 460 core

layout (location = 2) in vec3 oNormal;

layout (location = 0) out vec4 FragColor;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;

void main(){

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;

    vec3 result = ambient * u_objectColor;
    FragColor = vec4(result, 1.0);

}
