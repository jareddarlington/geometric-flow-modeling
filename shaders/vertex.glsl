#version 460 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal;

out vec3 normal; // output data; will be interpolated for each fragment

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
	normal = vertexNormal;
}