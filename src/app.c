#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "shader.h"
#include "camera.h"
#include "model.h"

#include <cglm/cglm.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

// TODO: Look more into computing geometric flows on discrete surfaces
// TODO: Spawn object not on top of player (or vice versa idk)

/*
 * Constants
 */

#define TITLE "Geometric Flow Modeling"
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

/*
 * Function Prototypes
 */

void error_callback(int error, const char *description);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main(void)
{
    /*
     * GLFW Setup
     */

    // Set GLFW error callback
    glfwSetErrorCallback(error_callback);

    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    // Set OpenGL version
    glfwWindowHint(GLFW_SAMPLES, 4);                               // 4x anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                 // 4.x major version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);                 // x.6 minor version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core OpenGL profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // fixes macOS issues

    // Create window
    GLFWwindow *window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, TITLE, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "Failed to create GLFW window\n");
        exit(EXIT_FAILURE);
    }

    // GLFW settings
    glfwMakeContextCurrent(window);                                    // make the window's context current
    glfwSetKeyCallback(window, key_callback);                          // set close on escape
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // callback function for frame resizing
    glfwSwapInterval(1);                                               // set buffer swap timing

    /*
     * OpenGL / Glad Setup
     */

    // Load OpenGL functions using Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize OpenGL context\n");
        exit(EXIT_FAILURE);
    }

    // OpenGL settings
    glClearColor(0.1686f, 0.1608f, 0.1686f, 1.0f); // set background colors
    glEnable(GL_DEPTH_TEST);                       // enable depth test (z-buffer)
    glDepthFunc(GL_LESS);                          // use fragment closer to the camera
    // glEnable(GL_CULL_FACE);                        // enable face culling (doesn't render polygons that aren't visible) // TODO: Enable later

    /*
     * Initialization
     */

    // Shaders and meshes
    GLuint shaderProgram = createShaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");

    Mesh *mesh = createMesh("models/sphere.obj");

    // MVP matrix
    GLuint mvpUniform = glGetUniformLocation(shaderProgram, "MVP"); // grab handle for MVP uniform
    mat4 mvp;                                                       // matrix to compute

    // Create camera
    vec3 initPos = {0.0f, 0.0f, 0.0f};
    Camera *camera = createCamera(initPos);

    /*
     * Rendering Loop
     */

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen

        glUseProgram(shaderProgram); // set shader program

        updateCamera(window, camera, mvp);                       // compute new mvp
        glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &mvp[0][0]); // send mvp to the currently bound shader

        // glBindVertexArray(VAO);            // bind VAO
        // glDrawArrays(GL_TRIANGLES, 0, 36); // draw object

        glBindBuffer(GL_ARRAY_BUFFER, mesh->posVBO);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(mesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh->vertCount);
        // drawMesh(mesh, shaderProgram, GL_TRIANGLES, containerPosition, rotation, CONTAINER_RADIUS * 2 + VERLET_RADIUS * 3);

        // glUseProgram(0);

        glBindVertexArray(0); // unbind VAO for cleanup

        glfwSwapBuffers(window); // swap front and back buffers
        glfwPollEvents();        // handles user input and window events
    }

    // Cleanup
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height); // set viewport to new window dimensions on resize
}