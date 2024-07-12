#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#include "model.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cglm/cglm.h>

// TODO: Keep track of faces for use in geometric flows (array or dynamic array or something else, this is a little bit tricky i think)

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

Mesh *createMesh(const char *filename)
{
    Mesh *mesh = malloc(sizeof(Mesh)); // allocate mesh memory
    loadOBJ(filename, mesh);

    // Vertex Array Object
    glGenVertexArrays(1, &(mesh->VAO));
    glBindVertexArray(mesh->VAO);

    // Vertex Buffer Object
    glGenBuffers(1, &(mesh->VBO));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * STRIDE * mesh->vertCount, mesh->vertices, GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void *)0);

    // Normal attribute (location = 1)
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void *)12);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return mesh;
}

void destroyMesh(Mesh *mesh)
{
    glDeleteVertexArrays(1, &(mesh->VAO));
    glDeleteBuffers(1, &(mesh->VBO));
    free(mesh);
}

void loadOBJ(const char *filename, Mesh *mesh)
{
    // Init arrays
    Vertex v[VERTEX_LIMIT];
    Vertex vn[VERTEX_LIMIT];
    Face faces[VERTEX_LIMIT];

    // Init counts
    int vCount = 0;
    int vnCount = 0;
    int fCount = 0;

    // Init vertices dynamic array
    DynamicArray *vertices = malloc(sizeof(DynamicArray));
    init(vertices, 160);

    FILE *fp = fopen(filename, "r"); // open file in read mode
    if (fp == NULL)
    {
        printf("Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1) // read each line in file
    {
        // Parse line
        char *words[4];
        words[0] = strtok(line, " "); // split line
        for (int i = 1; i < 4; ++i)
        {
            words[i] = strtok(NULL, " "); // NULL arg means continue tokenizing the same string
        }

        // Convert string values to floats and store them
        if (strcmp(words[0], "v") == 0) // vertex
        {
            v[vCount].x = atof(words[1]);
            v[vCount].y = atof(words[2]);
            v[vCount].z = atof(words[3]);
            vCount++;
        }
        else if (strcmp(words[0], "vn") == 0) // normal
        {
            vn[vnCount].x = atof(words[1]);
            vn[vnCount].y = atof(words[2]);
            vn[vnCount].z = atof(words[3]);
            vnCount++;
        }
        else if (strcmp(words[0], "f") == 0) // face
        {
            char *v1[3];
            char *v2[3];
            char *v3[3];

            v1[0] = strtok(words[1], "/"); // vertex index
            v1[1] = strtok(NULL, "/");     // texture index
            v1[2] = strtok(NULL, "/");     // normal index
            v2[0] = strtok(words[2], "/");
            v2[1] = strtok(NULL, "/");
            v2[2] = strtok(NULL, "/");
            v3[0] = strtok(words[3], "/");
            v3[1] = strtok(NULL, "/");
            v3[2] = strtok(NULL, "/");

            processVertex(vertices, v1, v, vn);
            processVertex(vertices, v2, v, vn);
            processVertex(vertices, v3, v, vn);

            // Change from 1 to 0 based index
            faces[fCount].v1 = atoi(v1[0]) - 1;
            faces[fCount].v2 = atoi(v2[0]) - 1;
            faces[fCount].v3 = atoi(v3[0]) - 1;
            fCount++;
        }
    }

    // Clean up
    fclose(fp);
    if (line)
    {
        free(line);
    }

    // Copy data into mesh
    mesh->vertices = vertices->array;
    mesh->vertCount = vertices->size / STRIDE; // equal to # of faces * # of vertices per face (3 here because of triangular faces) * # of dimensions (xyz so 3) / stride (3)
    mesh->faces = faces;
    mesh->faceCount = fCount;

    // DEBUG

    // printf("%d\n", mesh->vertCount);
    // for (int i = 0; i < mesh->vertCount; i++)
    // {
    //     printf("%f\n", mesh->vertices[i]);
    // }

    // printf("%d\n", fCount);
    // for (int i = 0; i < fCount; i++)
    //     printf("%d %d %d\n", faces[i].v1, faces[i].v2, faces[i].v3);
}

void processVertex(DynamicArray *vertices, char *vertexData[3], Vertex v[], Vertex vn[])
{
    // Change from 1 to 0 based index
    int vertexPtr = atoi(vertexData[0]) - 1; // ASCII to integer
    int normalPtr = atoi(vertexData[1]) - 1;

    push(vertices, v[vertexPtr].x);
    push(vertices, v[vertexPtr].y);
    push(vertices, v[vertexPtr].z);

    // push(vertices, vn[normalPtr].x);
    // push(vertices, vn[normalPtr].y);
    // push(vertices, vn[normalPtr].z);
}