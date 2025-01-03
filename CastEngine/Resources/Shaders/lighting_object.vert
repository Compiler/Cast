#version 460 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec4 aTexCoords;
layout (location = 3) in vec4 aNormal;

layout (location = 0) out vec4 oColor;
layout (location = 1) out vec4 oTexCoords;
layout (location = 2) out vec3 oNormal;


uniform mat4 iModel, iView, iProjection;

void main(){
    oTexCoords = aTexCoords;
    oColor = aColor;
    oNormal = aNormal.xyz;
    gl_Position = iProjection * iView * iModel * aPos;
}
