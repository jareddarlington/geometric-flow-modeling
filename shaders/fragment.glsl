#version 460 core

in vec3 fragNormal;

out vec4 color;

void main(){
	color = vec4(fragNormal * 0.5 + 0.5, 1.0);
}