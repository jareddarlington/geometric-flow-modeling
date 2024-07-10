#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

/*
 * Function Prototypes
 */

/**
 * @brief Parses shader file.
 *
 * @param shaderFile Shader file (.glsl, etc...).
 */
char *readShaderSource(const char *shaderFile);

/**
 * @brief Compiles shader
 *
 * @param shaderSource Shader to compile.
 * @param shaderType   Type of shader.
 */
GLuint compileShader(const char *shaderSource, GLenum shaderType);

/**
 * @brief Reads and compiles shader.
 *
 * @param vertexFilePath   Vertex shader file path.
 * @param fragmentFilePath Fragment shader file path.
 */
GLuint createShaderProgram(const char *vertexFilePath, const char *fragmentFilePath);

#endif