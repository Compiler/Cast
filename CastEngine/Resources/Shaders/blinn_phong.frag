#version 460 core

layout (location = 0) out vec4 FragColor;

uniform vec4 objectColor;
uniform vec4 lightColor;

void main(){

    FragColor = lightColor * objectColor;

}