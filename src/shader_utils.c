#include "shader_utils.h"
#include <stdio.h>
#include <stdlib.h>

char *readShaderSource(const char *shaderFile)
{
    FILE *fp = fopen(shaderFile, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open shader file: %s\n", shaderFile);
        return NULL;
    }
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
    fread(buf, 1, size, fp);
    buf[size] = '\0';
    fclose(fp);
    return buf;
}

GLuint compileShader(const char *shaderSource, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

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
    char *vertexShaderSource = readShaderSource(vertexFilePath);
    char *fragmentShaderSource = readShaderSource(fragmentFilePath);

    if (!vertexShaderSource || !fragmentShaderSource)
    {
        return 0;
    }

    GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    free(vertexShaderSource);
    free(fragmentShaderSource);

    if (!vertexShader || !fragmentShader)
    {
        return 0;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Error linking shader program: %s\n", infoLog);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
