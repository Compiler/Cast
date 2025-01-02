#version 460 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec4 aTexCoords;

layout (location = 0) out vec4 oColor;
layout (location = 1) out vec4 oTexCoords;


uniform mat4 u_model;

void main(){
    oTexCoords = aTexCoords;
    oColor = aColor;
    gl_Position = u_model * vec4(aPos.xyz, 1.0);
}
