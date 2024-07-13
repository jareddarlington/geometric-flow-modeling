#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "geometry.h"
#include "model.h"

#include <cglm/cglm.h>

void shrinkGeometry(GLFWwindow *window, Model *model)
{
    static double lastTime = 0.0;
    if (lastTime == 0.0)
        lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - lastTime);

    for (int i = 0; i < model->mesh->vertCount; i += 3)
    {
        // printf("%f\n", model->mesh->vertices[i]);
        // model->mesh->vertices[i] += cos(currentTime) * 2.0f * deltaTime;
        // model->mesh->vertices[i] += 2.0f * deltaTime;
    }

    // Rebind
    // glGenVertexArrays(1, &(mesh->VAO));
    // glBindVertexArray(mesh->VAO);

    // Vertex Buffer Object
    // glGenBuffers(1, &(mesh->VBO));
    glBindBuffer(GL_ARRAY_BUFFER, model->mesh->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * STRIDE * model->mesh->vertCount, model->mesh->vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * STRIDE * mesh->vertCount, mesh->vertices, GL_STATIC_DRAW);

    // Position attribute (location = 0)
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void *)0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // Last line
    lastTime = currentTime; // update last time taken
}