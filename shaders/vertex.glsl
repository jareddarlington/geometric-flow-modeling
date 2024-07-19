#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aCurvature;

out vec3 fragNormal;
out vec3 fragCurvature;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(aPosition, 1.0);
	fragNormal = aNormal;
	fragCurvature = aCurvature;
}