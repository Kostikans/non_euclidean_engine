#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aNormal;

out vec4 pos;
out vec4 normal;
out vec2 texCoord;
out vec4 pos_global;

uniform mat4 R, T, V, P; // Rotate,Translate,View,Project

void main() {
    mat4 modelView = V * T;
    pos_global = modelView * aPos;
    gl_Position = P * modelView * aPos;
    pos = aPos;
    normal = aNormal;
    texCoord = aTexCoord;
}