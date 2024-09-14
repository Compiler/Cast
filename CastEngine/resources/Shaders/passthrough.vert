#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

layout (location = 0) out vec3 oColor;
layout (location = 1) out vec2 oTexCoords;

void main(){
    oTexCoords = aTexCoords;
    oColor = aColor;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
