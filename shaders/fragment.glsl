//// OpenGL Version
//#version 460 core
//
//// Input
//in vec3 color;
//
//// Ouput
//out vec4 FragColor;
//
//void main() {
//    FragColor = vec4(color, 1.0);
//}

#version 460 core

// Output data
out vec3 color;

void main()
{

	// Output color = red
	color = vec3(1,0,0);

}