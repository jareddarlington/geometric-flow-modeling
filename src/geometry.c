#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "geometry.h"
#include "model.h"

#include <cglm/cglm.h>

void computeGeometry(GLFWwindow *window, Model *model)
{
    // for (int i = 0; i < 8; i++)
    //     printf("%f %f %f\n", model->mesh->vertices[i].position[0], model->mesh->vertices[i].position[1], model->mesh->vertices[i].position[2]);

    // for (int i = 0; i < 8; i++)
    //     printf("%f %f %f\n", model->mesh->vertices[i].normal[0], model->mesh->vertices[i].normal[1], model->mesh->vertices[i].normal[2]);

    // for (int i = 0; i < 36; i++)
    //     printf("%d\n", model->mesh->indices[i]);
    // printf("\n");

    glBindBuffer(GL_ARRAY_BUFFER, model->mesh->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(model->mesh->vertices), model->mesh->vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}