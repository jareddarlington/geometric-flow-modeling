#ifndef CAMERA_H
#define CAMERA_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#define SPEED 5.0f
#define MOUSE_SPEED 0.005f

/**
 * @brief Holds camera information.
 */
typedef struct
{
    vec3 position; // camera position
    float yaw;     // horizontal angle
    float pitch;   // vertical angle
    float fov;     // field of view
    vec3 front;    // direction of camera
    vec3 right;    // right of camera
    vec3 up;       // up from camera
} Camera;

/*
 * Functions
 */

/**
 * @brief Initializes camera object.
 *
 * @param position Initial position of camera.
 */
Camera *createCamera(GLFWwindow *window, vec3 position);

/**
 * @brief Recomputes front, right, and up vectors for camera.
 *
 * @param camera Camera to update.
 */
void updateVectors(Camera *camera);

/**
 * @brief Recomputes MVP based on user input and camera info.
 *
 * @param window GLFW window.
 * @param camera Camera to update and read from.
 * @param mvp    MVP matrix to update.
 */
void updateCamera(GLFWwindow *window, Camera *camera, mat4 mvp);

#endif