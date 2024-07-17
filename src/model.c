#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj.h"

#include "model.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include <cglm/cglm.h>

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
    // Allocate memory for mesh
    Mesh *mesh = malloc(sizeof(Mesh));

    // OBJ
    loadOBJ(filename, mesh);

    // VAO
    glGenVertexArrays(1, &mesh->VAO);
    glBindVertexArray(mesh->VAO);

    // VBO
    glGenBuffers(1, &mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * VERTEX_LIMIT, NULL, GL_DYNAMIC_DRAW);

    // Attributes
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(1); // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    // IBO
    glCreateBuffers(1, &mesh->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->numIndices * sizeof(uint32_t), mesh->indices, GL_STATIC_DRAW);

    return mesh;
}

void destroyMesh(Mesh *mesh)
{
    glDeleteVertexArrays(1, &(mesh->VAO));
    glDeleteBuffers(1, &(mesh->VBO));
    glDeleteBuffers(1, &(mesh->IBO));
    free(mesh->vertices);
    free(mesh->indices);
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

void loadOBJ(const char *filename, Mesh *mesh)
{
    fastObjMesh *obj = fast_obj_read(filename);
    if (!obj)
    {
        fprintf(stderr, "Error loading OBJ file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    mesh->numVertices = obj->position_count;
    mesh->numIndices = obj->face_count * 3; // Assuming triangles

    // Allocate memory for vertices and normals
    mesh->vertices = malloc(mesh->numVertices * sizeof(Vertex));
    mesh->indices = malloc(mesh->numIndices * sizeof(uint32_t));

    // Copy vertices and normals
    for (unsigned int i = 0; i < obj->position_count; ++i)
    {
        mesh->vertices[i].position[0] = obj->positions[3 * i + 0];
        mesh->vertices[i].position[1] = obj->positions[3 * i + 1];
        mesh->vertices[i].position[2] = obj->positions[3 * i + 2];

        mesh->vertices[i].normal[0] = obj->normals[3 * i + 0];
        mesh->vertices[i].normal[1] = obj->normals[3 * i + 1];
        mesh->vertices[i].normal[2] = obj->normals[3 * i + 2];
    }

    // Copy indices (converting to 0-based)
    for (unsigned int i = 0; i < obj->face_count; ++i)
    {
        mesh->indices[3 * i + 0] = obj->indices[3 * i + 0].p;
        mesh->indices[3 * i + 1] = obj->indices[3 * i + 1].p;
        mesh->indices[3 * i + 2] = obj->indices[3 * i + 2].p;
    }

    fast_obj_destroy(obj);
}