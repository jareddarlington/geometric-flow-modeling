#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

char *readShaderSource(const char *shaderFile)
{
    // Open shader file
    FILE *fp = fopen(shaderFile, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open shader file: %s\n", shaderFile);
        return NULL;
    }

    // Create buffer
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    char *buf = (char *)malloc(size + 1);
    if (buf == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for shader source\n");
        fclose(fp);
        return NULL;
    }

    // Fill buffer
    fread(buf, 1, size, fp);
    buf[size] = '\0';

    // Close file
    fclose(fp);
    return buf;
}

GLuint compileShader(const char *shaderSource, GLenum shaderType)
{
    // Create and compile shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // Check compile status
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Error compiling shader: %s\n", infoLog);
        return 0;
    }

    return shader;
}

GLuint createShaderProgram(const char *vertexFilePath, const char *fragmentFilePath)
{
    // Read shaders
    char *vertexShaderSource = readShaderSource(vertexFilePath);
    char *fragmentShaderSource = readShaderSource(fragmentFilePath);
    if (!vertexShaderSource || !fragmentShaderSource)
    {
        return 0;
    }

    // Compile shaders
    GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    // Free buffer memory
    free(vertexShaderSource);
    free(fragmentShaderSource);

    if (!vertexShader || !fragmentShader)
    {
        return 0;
    }

    // Create, attach, and link shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check link status
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Error linking shader program: %s\n", infoLog);
        return 0;
    }

    // Delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}