#version 460 core

in vec3 fragNormal;
in vec3 fragCurvature;

out vec4 color;

void main() {
	//color = vec4(fragCurvature * 0.5 + 0.5, 1.0);

	float flowMagnitude = length(fragCurvature);
	vec3 flowDirection = normalize(fragCurvature);
	vec3 fragColor = mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 0.0), flowMagnitude);
	color = vec4(fragColor, 1.0);
}