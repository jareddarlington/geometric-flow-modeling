#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "camera.h"
#include "model.h"

#include <cglm/cglm.h>

#include <stdio.h>

// TODO: Set rotation height, angle, etc to be based on model
// TODO: Fix issue with camera turning away from object while swapping camera modes

Camera *createCamera(GLFWwindow *window)
{
    // Grab init window info
    int winWidth, winHeight;
    glfwGetWindowSize(window, &winWidth, &winHeight);
    glfwSetCursorPos(window, winWidth / 2, winHeight / 2);

    // Malloc and set defaults
    Camera *camera = malloc(sizeof(Camera));
    glm_vec3_copy(INIT_CAMERA_POSITION, camera->position);
    camera->yaw = INIT_YAW;
    camera->pitch = INIT_PITCH;
    camera->fov = INIT_FOV;
    updateVectors(camera);
    camera->speed = INIT_MOVEMENT_SPEED;

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

void updateCamera(GLFWwindow *window, Camera *camera, mat4 pDest, mat4 vDest)
{
    // Calculate change in time
    static double lastTime = 0.0;
    if (lastTime == 0.0)
        lastTime = glfwGetTime();
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

    // Clamp camera
    if (camera->pitch > M_PI_2)
        camera->pitch = M_PI_2;
    else if (camera->pitch < -M_PI_2)
        camera->pitch = -M_PI_2;

    updateVectors(camera); // compute front, right, and up

    // UP/DOWN speed change
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera->speed += MOVEMENT_SPEED;
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera->speed -= MOVEMENT_SPEED;

    // Clamp speed
    if (camera->speed > MAX_MOVEMENT_SPEED)
        camera->speed = MAX_MOVEMENT_SPEED;
    if (camera->speed < MIN_MOVEMENT_SPEED)
        camera->speed = MIN_MOVEMENT_SPEED;

    // WASD movement
    vec3 temp;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forward
    {
        glm_vec3_scale(camera->front, deltaTime * camera->speed, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // back
    {
        glm_vec3_scale(camera->front, deltaTime * camera->speed, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right
    {
        glm_vec3_scale(camera->right, deltaTime * camera->speed, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left
    {
        glm_vec3_scale(camera->right, deltaTime * camera->speed, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }

    mat4 projection, view; // projection matrix, camera matrix
    vec3 posDestTemp;
    glm_vec3_add(camera->position, camera->front, posDestTemp);

    glm_perspective(glm_rad(camera->fov), // fov
                    ASPECT_RATIO,         // aspect ratio
                    NEAR_Z,               // near value
                    FAR_Z,                // far value
                    projection);          // destination
    glm_lookat(camera->position,          // camera position (world space)
               posDestTemp,               // direction
               camera->up,                // up
               view);                     // destination

    // Copy over matrices
    glm_mat4_copy(projection, pDest);
    glm_mat4_copy(view, vDest);

    lastTime = currentTime; // update last time taken
}

void updateRotationCamera(GLFWwindow *window, Camera *camera, Model *model, mat4 pDest, mat4 vDest)
{
    static float radius = INIT_RADIUS; // init radius

    // Calculate change in time
    static double lastTime = 0.0;
    if (lastTime == 0.0)
        lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - lastTime);

    // Stabilize camera
    camera->pitch = 0.0f;
    camera->yaw = 0.0f;

    // Rotate camera
    glm_vec3_add(model->position, (vec3){radius * cos(currentTime / 4.0f), 1.0f, radius * sin(currentTime / 4.0f)}, camera->position);
    updateVectors(camera); // compute front, right, and up

    // Zoom movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // in
        radius -= ZOOM_SPEED * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // out
        radius += ZOOM_SPEED * deltaTime;

    // Clamp zoom
    if (radius < 0.001f)
        radius = 0.001f;

    mat4 projection, view; // projection matrix, camera matrix

    glm_perspective(glm_rad(camera->fov), // fov
                    ASPECT_RATIO,         // aspect ratio
                    NEAR_Z,               // near value
                    FAR_Z,                // far value
                    projection);          // destination
    glm_lookat(camera->position,          // camera position (world space)
               model->position,           // direction
               camera->up,                // up
               view);                     // destination

    // Copy over matrices
    glm_mat4_copy(projection, pDest);
    glm_mat4_copy(view, vDest);

    lastTime = currentTime;
}