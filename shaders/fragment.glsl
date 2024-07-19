#version 460 core

in vec3 fragNormal;
in vec3 fragCurvature;

out vec4 color;

void main() {
	color = vec4(fragCurvature * 0.5 + 0.5, 1.0);
}