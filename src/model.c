#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#include "model.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include <cglm/cglm.h>

// TODO: Keep track of faces for use in geometric flows (array or dynamic array or something else, this is a little bit tricky i think)
// TODO: consider swaping to glDrawElements

Model *createModel(Mesh *mesh)
{
    Model *model = malloc(sizeof(Model));                // allocate model memory
    model->mesh = mesh;                                  // set mesh
    glm_vec3_copy(INIT_MODEL_POSITION, model->position); // set position
    glm_vec3_copy(INIT_ROTATION, model->rotation);       // set rotation
    glm_vec3_copy(INIT_SCALE, model->scale);             // set scale
    model->renderMethod = GL_TRIANGLES;                  // set render method
    return model;
}

void destroyModel(Model *model)
{
    destroyMesh(model->mesh); // destroy mesh
    free(model);              // free model memory
}

Mesh *createMesh(const char *filename)
{
    Mesh *mesh = malloc(sizeof(Mesh)); // allocate mesh memory
    // loadOBJ(filename, mesh);
    fakeLoad(mesh);

    // Vertex Array Object
    glCreateVertexArrays(1, &(mesh->VAO));
    // glGenVertexArrays(1, &mesh->VAO);
    glBindVertexArray(mesh->VAO);

    // Vertex Buffer Object
    glCreateBuffers(1, &(mesh->VBO));
    // glGenBuffers(1, &mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * VERTEX_LIMIT, NULL, GL_DYNAMIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, mesh->vertices, GL_STATIC_DRAW);

    // Position (location = 0)
    glEnableVertexArrayAttrib(mesh->VAO, 0);
    // glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

    // Normal (location = 1)
    glEnableVertexArrayAttrib(mesh->VAO, 1);
    // glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    // Index Buffer Object
    glCreateBuffers(1, &(mesh->IBO));
    // glGenBuffers(1, &mesh->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh->indices), mesh->indices, GL_STATIC_DRAW);

    return mesh;
}

void destroyMesh(Mesh *mesh)
{
    glDeleteVertexArrays(1, &(mesh->VAO));
    glDeleteBuffers(1, &(mesh->VBO));
    glDeleteBuffers(1, &(mesh->IBO));
    free(mesh);
}

void computeModelMatrix(Model *model, mat4 *dest)
{
    mat4 modelMatrix;
    glm_mat4_identity(modelMatrix);

    // Translation
    glm_translate(modelMatrix, model->position);

    // Rotation
    glm_rotate(modelMatrix, model->rotation[0], GLM_XUP);
    glm_rotate(modelMatrix, model->rotation[1], GLM_YUP);
    glm_rotate(modelMatrix, model->rotation[2], GLM_ZUP);

    // Scaling
    glm_scale(modelMatrix, model->scale);

    glm_mat4_copy(modelMatrix, *dest); // copy over computed vp
}

void fakeLoad(Mesh *mesh)
{
    Vertex vertices[8];

    glm_vec3_copy((vec3){-1.0f, -1.0f, -1.0f}, vertices[0].position);
    glm_vec3_copy((vec3){-1.0f, 0.0f, 0.0f}, vertices[0].normal);

    glm_vec3_copy((vec3){1.0f, -1.0f, -1.0f}, vertices[1].position);
    glm_vec3_copy((vec3){1.0f, 0.0f, 0.0f}, vertices[1].normal);

    glm_vec3_copy((vec3){1.0f, 1.0f, -1.0f}, vertices[2].position);
    glm_vec3_copy((vec3){-0.0f, 1.0f, -0.0f}, vertices[2].normal);

    glm_vec3_copy((vec3){-1.0f, 1.0f, -1.0f}, vertices[3].position);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, vertices[3].normal);

    glm_vec3_copy((vec3){-1.0f, -1.0f, 1.0f}, vertices[4].position);
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, vertices[4].normal);

    glm_vec3_copy((vec3){1.0f, -1.0f, 1.0f}, vertices[5].position);
    glm_vec3_copy((vec3){0.0f, 0.0f, 1.0f}, vertices[5].normal);

    glm_vec3_copy((vec3){1.0f, 1.0f, 1.0f}, vertices[6].position);
    glm_vec3_copy((vec3){0.0f, 0.0f, 1.0f}, vertices[6].normal);

    glm_vec3_copy((vec3){-1.0f, 1.0f, 1.0f}, vertices[7].position);
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, vertices[7].normal);

    mesh->vertices = malloc(8 * sizeof(Vertex));
    memcpy(mesh->vertices, vertices, 8 * sizeof(Vertex));

    unsigned int indices[] = {
        0, 1, 2, 0, 2, 3, // Front face
        1, 5, 6, 1, 6, 2, // Right face
        5, 4, 7, 5, 7, 6, // Back face
        4, 0, 3, 4, 3, 7, // Left face
        3, 2, 6, 3, 6, 7, // Top face
        4, 5, 1, 4, 1, 0  // Bottom face
    };

    mesh->indices = malloc(36 * sizeof(unsigned int));
    memcpy(mesh->indices, indices, 36 * sizeof(unsigned int));
}