#version 410 core

layout (location = 0) in vec3 oColor;
layout (location = 1) in vec2 oTexCoords;

layout (location = 0) out vec4 FragColor;

uniform float u_time;
uniform sampler2D u_texture;

void main(){

    //FragColor = vec4(oColor.r, u_time * sin(oColor.g), oColor.b, 1.0f);
    FragColor = texture(u_texture, oTexCoords); 

}
