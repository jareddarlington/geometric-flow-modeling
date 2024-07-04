#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"
#include "shader_utils.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

// Function prototypes
void error_callback(int error, const char *description);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// Settings
static const unsigned int DEFAULT_WIDTH = 800;
static const unsigned int DEFAULT_HEIGHT = 600;
static const char *TITLE = "Geometric Flow Modeling";

int main(void)
{
    // Set GLFW error callback
    glfwSetErrorCallback(error_callback);

    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    // Create a windowed mode window and its OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, TITLE, NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "Failed to create GLFW window\n");
        exit(EXIT_FAILURE);
    }

    // Set close on escape
    glfwSetKeyCallback(window, key_callback);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load OpenGL functions using glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize OpenGL context\n");
        exit(EXIT_FAILURE);
    }

    // Set buffer swap timing
    glfwSwapInterval(1);

    GLuint shaderProgram = createShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl");
    if (!shaderProgram)
    {
        return -1;
    }

    float vertices[] = {
        -1.0f, -1.0f, 0.0f, // Vertex 1 position (x, y, z)
        1.0f, -1.0f, 0.0f,  // Vertex 2 position (x, y, z)
        0.0f, 1.0f, 0.0f    // Vertex 3 position (x, y, z)
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}