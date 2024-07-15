#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

out vec3 v_Normal;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(a_Position, 1.0);
	v_Normal = a_Normal;
}