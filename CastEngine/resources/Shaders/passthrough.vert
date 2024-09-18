#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aTexCoords;

layout (location = 0) out vec3 oColor;
layout (location = 1) out vec3 oTexCoords;

uniform mat4 u_projection;

void main(){
    oTexCoords = aTexCoords;
    oColor = aColor;
    gl_Position = u_projection * vec4(aPos.xy, 1.0f, 1.0);
}
