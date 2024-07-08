#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "shader.h"
#include "camera.h"

#include <cglm/cglm.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

// TODO: Look more into computing geometric flows on discrete surfaces

// Function prototypes
void error_callback(int error, const char *description);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// Settings
static const unsigned int DEFAULT_WIDTH = 800;
static const unsigned int DEFAULT_HEIGHT = 600;
static const char *TITLE = "Geometric Flow Modeling";

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    glClearColor(0.1686f, 0.1608f, 0.1686f, 1.0f); // set background color
    glEnable(GL_DEPTH_TEST);                       // enable depth test (z-buffer)
    glDepthFunc(GL_LESS);                          // use fragment closer to the camera

    /*
     * Initialization
     */

    // Create shader
    GLuint shaderProgram = createShaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");

    static const GLfloat vertices[] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};
    static const GLfloat colors[] = {
        0.583f, 0.771f, 0.014f,
        0.609f, 0.115f, 0.436f,
        0.327f, 0.483f, 0.844f,
        0.822f, 0.569f, 0.201f,
        0.435f, 0.602f, 0.223f,
        0.310f, 0.747f, 0.185f,
        0.597f, 0.770f, 0.761f,
        0.559f, 0.436f, 0.730f,
        0.359f, 0.583f, 0.152f,
        0.483f, 0.596f, 0.789f,
        0.559f, 0.861f, 0.639f,
        0.195f, 0.548f, 0.859f,
        0.014f, 0.184f, 0.576f,
        0.771f, 0.328f, 0.970f,
        0.406f, 0.615f, 0.116f,
        0.676f, 0.977f, 0.133f,
        0.971f, 0.572f, 0.833f,
        0.140f, 0.616f, 0.489f,
        0.997f, 0.513f, 0.064f,
        0.945f, 0.719f, 0.592f,
        0.543f, 0.021f, 0.978f,
        0.279f, 0.317f, 0.505f,
        0.167f, 0.620f, 0.077f,
        0.347f, 0.857f, 0.137f,
        0.055f, 0.953f, 0.042f,
        0.714f, 0.505f, 0.345f,
        0.783f, 0.290f, 0.734f,
        0.722f, 0.645f, 0.174f,
        0.302f, 0.455f, 0.848f,
        0.225f, 0.587f, 0.040f,
        0.517f, 0.713f, 0.338f,
        0.053f, 0.959f, 0.120f,
        0.393f, 0.621f, 0.362f,
        0.673f, 0.211f, 0.457f,
        0.820f, 0.883f, 0.371f,
        0.982f, 0.099f, 0.879f};

    GLuint mvpUniform = glGetUniformLocation(shaderProgram, "MVP");
    mat4 mvp; // mvp matrix
    // mat4 projection; // projection matrix
    // mat4 view;       // camera matrix
    // mat4 model;      // model matrix

    // glm_perspective(glm_rad(60.0f), 4.0f / 3.0f, 0.1f, 100.0f, projection); // fov y, aspect ratio, near value, far value, destination
    // glm_lookat((vec3){4.0f, 3.0f, -3.0f},                                   // camera position (world space)
    //            (vec3){0.0f, 0.0f, 0.0f},                                    // camera looks here (world space)
    //            (vec3){0.0f, 1.0f, 0.0f},                                    // head is up ({0, -1, 0} to look upside down)
    //            view);                                                       // destination
    // glm_mat4_identity(model);                                               // identity matrix for model
    // glm_mat4_mul(projection, view, mvp);                                    // mvp = projection * view
    // glm_mat4_mul(mvp, model, mvp);                                          // mvp = mvp * model

    // Vertex Array Object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Buffer Object
    GLuint VBO;
    glGenBuffers(1, &VBO);                                                     // gen buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                        // bind buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // allocate memory and upload vertex data to GPU

    // Vertex/position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Color Buffer Object
    GLuint CBO;
    glGenBuffers(1, &CBO);                                                 // gen buffer
    glBindBuffer(GL_ARRAY_BUFFER, CBO);                                    // bind buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW); // allocate memory and upload color data to GPU

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // unbind VAO for cleanup

    vec3 initPos = {0.0f, 0.0f, 5.0f};
    Camera *camera = createCamera(initPos);

    /*
     * Rendering Loop
     */

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen

        glUseProgram(shaderProgram); // set shader program

        updateCamera(window, camera, mvp); // compute new mvp

        glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &mvp[0][0]); // send mvp transformation to the currently bound shader

        glBindVertexArray(VAO);            // bind VAO
        glDrawArrays(GL_TRIANGLES, 0, 36); // draw object

        glBindVertexArray(0); // unbind VAO for cleanup

        glfwSwapBuffers(window); // swap front and back buffers
        glfwPollEvents();        // handles user input and window events
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
    glViewport(0, 0, width, height); // set viewport to new window dimensions
}