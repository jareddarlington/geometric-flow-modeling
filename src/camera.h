

#ifndef CAMERA_H
#define CAMERA_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

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

// Functions
Camera *createCamera(vec3 position);
void updateVectors(Camera *camera);
mat4 *computeMVP(GLFWwindow *window, Camera *camera);

#endif