#version 460 core

in vec3 fragNormal;
in vec3 fragCurvature;

out vec4 color;

void main() {
	float flowMagnitude = length(fragCurvature);
	vec3 fragColor = mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 0.0), flowMagnitude);
	color = vec4(fragColor, 1.0);
}