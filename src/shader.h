#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

// Functions
char *readShaderSource(const char *shaderFile);
GLuint compileShader(const char *shaderSource, GLenum shaderType);
GLuint createShaderProgram(const char *vertexFilePath, const char *fragmentFilePath);

#endif