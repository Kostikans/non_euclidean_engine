#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;

uniform mat4 R, T, V, P; // Rotate,Translate,View,Project

void main() {
    gl_Position = P * V * T * aPos;
}