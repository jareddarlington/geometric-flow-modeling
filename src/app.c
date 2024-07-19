#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "geometry.h"

#include <cglm/cglm.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

// TODO: Fix camera turning away from object while going from rotate mode to free mode, I want the camera direction to be the same when transitioning
// TODO: Make readme and add info about each geometric flow and its algorithm
// TODO: seperate m, v, and p matrices until shader compilation

/*
 * Constants
 */

#define TITLE "Geometric Flow Modeling" // window title
#define DEFAULT_WIDTH 800               // default window width
#define DEFAULT_HEIGHT 600              // default window height
#define ANTI_ALIASING 4                 // anti-aliasing quality
#define MAJOR_VERSION 4                 // OpenGL major version
#define MINOR_VERION 6                  // OpenGL minor version

/*
 * Enums
 */

typedef enum
{
    ROTATE,
    FREE,
    LOCK
} CAMERA_MODE;

/*
 * Function Prototypes
 */

void error_callback(int error, const char *description);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

/*
 * Globals
 */

CAMERA_MODE cMode = FREE;  // initial camera mode
GEOMETRIC_FLOW flow = MCF; // geometric flow to compute
bool flowing = false;

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
    glfwWindowHint(GLFW_SAMPLES, ANTI_ALIASING);                   // anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);     // major version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERION);      // minor version
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
    glfwSwapInterval(1);                                               // set buffer swap timing (vsync enabled)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);         // set cursor to hidden

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
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);     // enable polygon mode globally
    // glEnable(GL_CULL_FACE);                        // enable face culling (skips rendering non-visible polygons)

    /*
     * Initialization
     */

    // Shaders and meshes
    GLuint shaderProgram = createShaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");

    Mesh *mesh = createMesh("models/high_density_icosphere.obj");
    Model *model = createModel(mesh);

    // Transformations
    Camera *camera = createCamera(window);
    mat4 m;
    mat4 v;
    mat4 p;

    /*
     * Rendering Loop
     */

    while (!glfwWindowShouldClose(window))
    {
        // Dynamically update geometry
        computeGeometry(window, model, flow, flowing);

        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Shaders
        glUseProgram(shaderProgram);

        // Camera
        switch (cMode)
        {
        case ROTATE:
            updateRotationCamera(window, camera, model, p, v);
            break;
        case FREE:
            updateCamera(window, camera, p, v);
            break;
        case LOCK:
            break;
        }

        // MVP
        computeModelMatrix(model, &m);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &m[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &v[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &p[0][0]);

        // Draw
        glBindVertexArray(model->mesh->VAO);
        glDrawElements(model->renderMethod, model->mesh->numIndices, GL_UNSIGNED_INT, NULL);

        // Buffer swapping and event handling
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroyModel(model);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

/**
 * @brief GLFW error callback function.
 *
 * @param error       Error number.
 * @param description Description of error.
 */
void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

/**
 * @brief GLFW key callback function.
 *
 * @param window   The window that received the event.
 * @param key      The keyboard key that was pressed or released.
 * @param scancode The platform-specific scancode of the key.
 * @param action   The key action.
 * @param mods     Bit field describing which modifier keys were held down.
 */
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // close on escape
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_C && action == GLFW_PRESS) // turn camera mode on/off
    {
        switch (cMode)
        {
        case ROTATE:
            cMode = FREE;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        case FREE:
            cMode = LOCK;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case LOCK:
            cMode = ROTATE;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        }
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        flowing = !flowing;
}

/**
 * @brief GLFW framebuffer size callback; called on frame resize.
 *
 * @param window The window whose framebuffer was resized.
 * @param width  The new width, in pixels, of the framebuffer.
 * @param heigth The new height, in pixels, of the framebuffer.
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height); // set viewport to new window dimensions
}