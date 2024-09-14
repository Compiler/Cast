#version 410 core

layout (location = 0) in vec3 oColor;

layout (location = 0) out vec4 FragColor;

uniform float u_time;

void main(){

    FragColor = vec4(oColor.r, u_time * sin(oColor.g), oColor.b, 1.0f);

}
