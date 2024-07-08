#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "shader.h"
#include "model.h"
#include <cglm/cglm.h>

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

    // Set background color
    glClearColor(0.1686f, 0.1608f, 0.1686f, 1.0f);

    // Set buffer swap timing
    glfwSwapInterval(1);

    GLuint shaderProgram = createShaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");
    if (!shaderProgram)
    {
        return -1;
    }

    static const GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, // bottom left
        1.0f, -1.0f, 0.0f,  // bottom right
        0.0f, 1.0f, 0.0f    // top middle
    };

    GLuint mvpUniform = glGetUniformLocation(shaderProgram, "MVP");
    mat4 mvp;        // mvp matrix
    mat4 projection; // projection matrix
    mat4 view;       // camera matrix
    mat4 model;      // model matrix

    glm_perspective(glm_rad(45.0f), 2.0f / 3.0f, 0.1f, 100.0f, projection); // fov y, aspect ratio, near value, far value, destination
    glm_lookat((vec3){4.0f, 3.0f, 3.0f},                                    // camera position (world space)
               (vec3){0.0f, 0.0f, 0.0f},                                    // camera looks here (world space)
               (vec3){0.0f, 1.0f, 0.0f},                                    // head is up ({0, -1, 0} to look upside down)
               view);                                                       // destination
    glm_mat4_identity(model);                                               // identity matrix for model
    glm_mat4_mul(projection, view, mvp);                                    // mvp = projection * view
    glm_mat4_mul(mvp, model, mvp);                                          // mvp = mvp * model

    // Create Vertex Array Object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create Vertex Buffer Object
    GLuint VBO;
    glGenBuffers(1, &VBO);                                                     // gen buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                        // bind buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // allocate memory and upload vertex data to GPU

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // unbind VAO for cleanup

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen

        glUseProgram(shaderProgram); // set shader program

        glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &mvp[0][0]); // send transformation to the currently bound shader

        glBindVertexArray(VAO);           // bind VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // draw object

        glBindVertexArray(0); // unbind VAO for cleanup

        glfwSwapBuffers(window); // swap front and back buffers
        glfwPollEvents();        // handles user input and window events
    }

    // Cleanup
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