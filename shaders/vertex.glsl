#version 460 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 fragNormal;

uniform mat4 MVP;

void main(){
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	fragNormal = vertexNormal;
}