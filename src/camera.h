#ifndef CAMERA_H
#define CAMERA_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "model.h"

#include <cglm/cglm.h>

// TODO: update docs

// Movement settings
#define INIT_SPEED 5.0f    // camera movement speed
#define MOUSE_SPEED 0.005f // turn / look speed
#define ZOOM_SPEED 5.0f    // change of radius speed for rotate mode

// Camera position settings
#define INIT_CAMERA_POSITION \
    (vec3) { 0.0f, 0.0f, -3.0f } // camera inital position
#define INIT_YAW 0               // camera inital yaw / horizontal angle
#define INIT_PITCH 0             // camera inital pitch / vertical angle
#define INIT_FOV 90              // camera init fov
#define INIT_RADIUS 3.0f         // distance away from object in rotate mode

// Camera settings
#define ASPECT_RATIO 4.0f / 3.0f // camera aspect ratio
#define NEAR_Z 0.001f            // near value for z buffer (clipping plane)
#define FAR_Z 100.0f             // far value for z buffer (clipping plane)

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
    float speed;
} Camera;

/*
 * Function Prototypes
 */

/**
 * @brief Initializes camera object.
 *
 * @param window GLFW window.
 */
Camera *createCamera(GLFWwindow *window);

/**
 * @brief Recomputes front, right, and up vectors for camera.
 *
 * @param camera Camera to update.
 */
void updateVectors(Camera *camera);

/**
 * @brief Recomputes VP based on user input and camera info.
 *
 * @param window GLFW window.
 * @param camera Camera to update and read from.
 * @param vp     VP matrix to update.
 */
void updateCamera(GLFWwindow *window, Camera *camera, mat4 pDest, mat4 vDest);

/**
 * @brief Recomputes VP based on user input, time, and model info.
 *
 * @param window GLFW window.
 * @param camera Camera to update and read from.
 * @param vp     VP matrix to update.
 * @param model  Model to rotate around.
 */
void updateRotationCamera(GLFWwindow *window, Camera *camera, Model *model, mat4 pDest, mat4 vDest);

#endif