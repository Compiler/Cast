#version 460 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec4 aTexCoords;

layout (location = 0) out vec4 oColor;
layout (location = 1) out vec4 oTexCoords;

void main(){
    oTexCoords = aTexCoords;
    oColor = aColor;
    gl_Position = vec4(aPos.xyz, 1.0);
}
