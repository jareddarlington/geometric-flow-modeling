#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "camera.h"
#include "model.h"

#include <cglm/cglm.h>

#include <stdio.h>

// TODO: Slow rotation speed
// TODO: fix rotation height, angle, etc...

Camera *createCamera(GLFWwindow *window)
{
    int winWidth, winHeight;
    glfwGetWindowSize(window, &winWidth, &winHeight);
    glfwSetCursorPos(window, winWidth / 2, winHeight / 2);

    Camera *camera = malloc(sizeof(Camera));
    glm_vec3_copy(INIT_CAMERA_POSITION, camera->position);
    camera->yaw = INIT_YAW;
    camera->pitch = INIT_PITCH;
    camera->fov = INIT_FOV;
    updateVectors(camera);
    camera->speed = INIT_SPEED;
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

    // Clamp camera
    if (camera->pitch > M_PI_2)
        camera->pitch = M_PI_2;
    else if (camera->pitch < -M_PI_2)
        camera->pitch = -M_PI_2;

    updateVectors(camera); // compute front, right, and up

    // UP/DOWN speed change
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera->speed += 0.1f;
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera->speed -= 0.1f;

    // Clamp speed
    if (camera->speed > 5.0f)
        camera->speed = 5.0f;
    if (camera->speed < 0.5f)
        camera->speed = 0.5f;

    // WASD movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forward
    {
        vec3 temp;
        glm_vec3_scale(camera->front, deltaTime * camera->speed, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // back
    {
        vec3 temp;
        glm_vec3_scale(camera->front, deltaTime * camera->speed, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right
    {
        vec3 temp;
        glm_vec3_scale(camera->right, deltaTime * camera->speed, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left
    {
        vec3 temp;
        glm_vec3_scale(camera->right, deltaTime * camera->speed, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }

    mat4 projection; // projection matrix
    mat4 view;       // camera matrix

    vec3 posDestTemp;
    glm_vec3_add(camera->position, camera->front, posDestTemp);

    glm_perspective(glm_rad(camera->fov), ASPECT_RATIO, NEAR_Z, FAR_Z, projection); // fov, aspect ratio, near value, far value, destination
    glm_lookat(camera->position,                                                    // camera position (world space)
               posDestTemp,                                                         // direction
               camera->up,                                                          // up
               view);                                                               // destination

    // Copy over matrices
    glm_mat4_copy(projection, pDest);
    glm_mat4_copy(view, vDest);

    lastTime = currentTime; // update last time taken
}

void updateRotationCamera(GLFWwindow *window, Camera *camera, Model *model, mat4 pDest, mat4 vDest)
{
    static float radius = INIT_RADIUS; // distance away from object

    static double lastTime = 0.0; // init time (first function call)

    if (lastTime == 0.0)
        lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - lastTime);

    camera->yaw = 0.0f;
    camera->pitch = 0.0f;
    glm_vec3_add(model->position, (vec3){radius * cos(glfwGetTime()), 1.0f, radius * sin(glfwGetTime())}, camera->position);
    updateVectors(camera); // compute front, right, and up

    // "Zoom" movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // in
        radius -= ZOOM_SPEED * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // out
        radius += ZOOM_SPEED * deltaTime;

    mat4 projection; // projection matrix
    mat4 view;       // camera matrix

    glm_perspective(glm_rad(camera->fov), ASPECT_RATIO, NEAR_Z, FAR_Z, projection); // fov, aspect ratio, near value, far value, destination
    glm_lookat(camera->position,                                                    // camera position (world space)
               model->position,                                                     // direction
               camera->up,                                                          // up
               view);                                                               // destination

    // Copy over matrices
    glm_mat4_copy(projection, pDest);
    glm_mat4_copy(view, vDest);

    lastTime = currentTime; // update last time taken
}