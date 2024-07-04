#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <glad/glad.h>

// Function declarations
char *readShaderSource(const char *shaderFile);
GLuint compileShader(const char *shaderSource, GLenum shaderType);
GLuint createShaderProgram(const char *vertexFilePath, const char *fragmentFilePath);

#endif // SHADER_UTILS_H
