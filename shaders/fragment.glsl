#version 460 core

in vec3 normal; // interpolated values from the vertex shaders

out vec4 color; // output data

void main()
{
	color = vec4(0.553, 0.388, 0.722, 1.0); // output color
}