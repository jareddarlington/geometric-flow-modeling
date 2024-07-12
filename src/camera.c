#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "camera.h"

#include <cglm/cglm.h>

#include <stdio.h>

// TODO: Create a seperate camera that rotates around the object ( position = ObjectCenter + ( radius * cos(time), height, radius * sin(time) ) ); bind the radius/height/time to the keyboard/mouse, or whatever

Camera *createCamera(GLFWwindow *window)
{
    int winWidth, winHeight;
    glfwGetWindowSize(window, &winWidth, &winHeight);
    glfwSetCursorPos(window, winWidth / 2, winHeight / 2);

    Camera *camera = malloc(sizeof(Camera));
    glm_vec3_copy(INIT_POSITION, camera->position);
    camera->yaw = INIT_YAW;
    camera->pitch = INIT_PITCH;
    camera->fov = INIT_FOV;
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

void updateCamera(GLFWwindow *window, Camera *camera, mat4 vp)
{
    static double lastTime = 0.0; // init time (first function call)

    if (lastTime == 0.0)
        lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position
    int winWidth, winHeight;
    glfwGetWindowSize(window, &winWidth, &winHeight);
    glfwSetCursorPos(window, winWidth / 2, winHeight / 2);

    // Compute new orientation
    camera->yaw += MOUSE_SPEED * ((winWidth / 2) - xpos);
    camera->pitch += MOUSE_SPEED * ((winHeight / 2) - ypos);
    if (camera->pitch > M_PI_2)
        camera->pitch = M_PI_2;
    if (camera->pitch < -M_PI_2)
        camera->pitch = -M_PI_2;

    updateVectors(camera); // compute front, right, and up

    // WASD movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forward
    {
        vec3 temp;
        glm_vec3_scale(camera->front, deltaTime * SPEED, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // back
    {
        vec3 temp;
        glm_vec3_scale(camera->front, deltaTime * SPEED, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right
    {
        vec3 temp;
        glm_vec3_scale(camera->right, deltaTime * SPEED, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left
    {
        vec3 temp;
        glm_vec3_scale(camera->right, deltaTime * SPEED, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }

    mat4 projection; // projection matrix
    mat4 view;       // camera matrix
    mat4 model;      // model matrix
    mat4 tempVP;     // temporary VP

    vec3 posDestTemp;
    glm_vec3_add(camera->position, camera->front, posDestTemp);

    glm_perspective(glm_rad(camera->fov), ASPECT_RATIO, NEAR_Z, FAR_Z, projection); // fov, aspect ratio, near value, far value, destination
    glm_lookat(camera->position,                                                    // camera position (world space)
               posDestTemp,                                                         // direction
               camera->up,                                                          // up
               view);                                                               // destination
    glm_mat4_identity(model);                                                       // identity matrix for model
    glm_mat4_mul(projection, view, tempVP);                                         // VP = projection * view

    glm_mat4_copy(tempVP, vp); // copy over computed vp

    lastTime = currentTime; // update last time taken
}