#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;


uniform mat4 R, T, V, P; // Rotate,Translate,View,Project

void main() {
    gl_Position = P * V * T * aPos;
}