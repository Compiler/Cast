#version 460 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec4 aTexCoords;

layout (location = 0) out vec4 oColor;

uniform mat4 iModel, iView, iProjection;

void main(){

    oColor = aColor;
    gl_Position = iProjection * iView * iModel * vec4(aPos.xyz, 1.0);
}
