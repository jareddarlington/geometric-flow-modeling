// OpenGL Version
#version 460 core

// Input
layout(location = 0) in vec3 aPos;

// Output
out vec3 color;

void main() {
    gl_Position = vec4(aPos, 1.0);
    color = vec3(1.0, 0.0, 0.0);
}