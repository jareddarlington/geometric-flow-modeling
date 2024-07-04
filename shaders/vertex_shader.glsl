#version 460 core

layout(location = 0) in vec3 aPos;

out vec3 ourColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = vec3(1.0, 0.5, 0.2);
}