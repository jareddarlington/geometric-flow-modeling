#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "camera.h"

#include <cglm/cglm.h>

float speed = 3.0f; // 3 units per second
float mouseSpeed = 0.005f;

mat4 mvp; // mvp matrix

Camera *createCamera(vec3 position)
{
    Camera *camera = malloc(sizeof(Camera));
    glm_vec3_copy(camera->position, position);
    camera->yaw = 0;
    camera->pitch = 0;
    camera->fov = 60;
    updateVectors(camera);
    return camera;
};

void updateVectors(Camera *camera)
{
    // Compute front vector
    glm_vec3_copy((vec3){cos(camera->pitch) * sin(camera->yaw),
                         sin(camera->pitch),
                         cos(camera->pitch) * cos(camera->yaw)},
                  camera->front);

    // Compute right vector
    glm_vec3_copy((vec3){sin(camera->yaw - M_PI_2),
                         0,
                         cos(camera->yaw - M_PI_2)},
                  camera->right);

    // Compute up vector
    glm_vec3_cross(camera->right, camera->front, camera->up);
}

mat4 *computeMVP(GLFWwindow *window, Camera *camera)
{
    float deltaTime = glfwGetTime(); // get time since last frame // TODO: Fix this?

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position
    int winWidth;
    int winHeight;
    glfwGetWindowSize(window, &winWidth, &winHeight); // TODO: FIX?
    glfwSetCursorPos(window, winWidth / 2, winHeight / 2);

    // Compute new orientation
    camera->yaw += mouseSpeed * (float)((winWidth / 2) - xpos);
    camera->pitch += mouseSpeed * (float)((winHeight / 2) - xpos);

    updateVectors(camera);

    // WASD movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forward
    {
        vec3 temp;
        glm_vec3_scale(camera->front, deltaTime * speed, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // back
    {
        vec3 temp;
        glm_vec3_scale(camera->front, deltaTime * speed, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right
    {
        vec3 temp;
        glm_vec3_scale(camera->right, deltaTime * speed, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left
    {
        vec3 temp;
        glm_vec3_scale(camera->right, deltaTime * speed, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }

    mat4 projection; // projection matrix
    mat4 view;       // camera matrix
    mat4 model;      // model matrix

    vec3 posDestTemp;
    glm_vec3_add(camera->position, camera->front, posDestTemp);

    glm_perspective(glm_rad(camera->fov), 4.0f / 3.0f, 0.1f, 100.0f, projection); // fov y, aspect ratio, near value, far value, destination
    glm_lookat(camera->position,                                                  // camera position (world space)
               posDestTemp,                                                       // camera looks here (world space)
               camera->up,                                                        // head is up ({0, -1, 0} to look upside down)
               view);                                                             // destination
    glm_mat4_identity(model);                                                     // identity matrix for model
    glm_mat4_mul(projection, view, mvp);                                          // mvp = projection * view
    glm_mat4_mul(mvp, model, mvp);                                                // mvp = mvp * model

    return &mvp;
}