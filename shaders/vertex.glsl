//// OpenGL Version
//#version 460 core
//
//// Input
//layout(location = 0) in vec3 aPos;
//
//// Output
//out vec3 color;
//
//void main() {
//    gl_Position = vec4(aPos, 1.0);
//    color = vec3(1.0, 0.0, 0.0);
//}

#version 460 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

}