#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#include "model.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cglm/cglm.h>

Model *createModel(Mesh *mesh)
{
    Model *model = malloc(sizeof(Model));                      // allocate model memory
    model->mesh = mesh;                                        // set mesh
    glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, model->position);  // set position
    glm_vec3_copy((vec3){0.0f, -M_PI, 0.0f}, model->rotation); // set rotation
    glm_vec3_copy((vec3){1.0f, 1.0f, 1.0f}, model->scale);     // set scale
    model->renderMethod = GL_TRIANGLES;                        // set render method
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
    glm_rotate(modelMatrix, model->rotation[0], (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(modelMatrix, model->rotation[1], (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(modelMatrix, model->rotation[2], (vec3){0.0f, 0.0f, 1.0f});

    // Scaling
    glm_scale(modelMatrix, model->scale);

    glm_mat4_copy(modelMatrix, *dest); // copy over computed vp
}

Mesh *createMesh(const char *filename)
{
    DynamicArray *dArray = loadOBJ(filename);
    // DynamicArray *trianglesDArray;
    // loadOBJ(filename, verticesDArray, trianglesDArray);

    Mesh *mesh = malloc(sizeof(Mesh));       // allocate mesh memory
    mesh->vertices = dArray->array;          // set vertices
    mesh->vertCount = dArray->size / STRIDE; // set vertices count

    // Vertex Array Object
    glGenVertexArrays(1, &(mesh->VAO));
    glBindVertexArray(mesh->VAO);

    // Vertex Buffer Object
    glGenBuffers(1, &(mesh->VBO));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * STRIDE * mesh->vertCount, mesh->vertices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void *)0);

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void *)12);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    return mesh;
}

void destroyMesh(Mesh *mesh)
{
    glDeleteVertexArrays(1, &(mesh->VAO));
    glDeleteBuffers(1, &(mesh->VBO));
    free(mesh);
}

// void loadOBJ(const char *filename, DynamicArray *verticesDest, DynamicArray *trianglesDest)
DynamicArray *loadOBJ(const char *filename)
{
    // Init arrays
    Vertex v[VERTEX_LIMIT];
    Vertex vt[VERTEX_LIMIT];
    Vertex vn[VERTEX_LIMIT];

    // Init counts
    int v_count = 0;
    int vt_count = 0;
    int vn_count = 0;

    // Init vertices dynamic array
    DynamicArray *vertices = malloc(sizeof(DynamicArray));
    init(vertices, 160);

    // Init triangles dynamic array
    // DynamicArray *triangles = malloc(sizeof(DynamicArray));
    // init(triangles, 160);

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
        words[0] = strtok(line, " "); // split line string
        for (int i = 1; i < 4; ++i)
        {
            words[i] = strtok(NULL, " "); // NULL arg means continue tokenizing the same string
        }

        // Convert string values to floats and store them
        if (strcmp(words[0], "v") == 0) // vertex
        {
            v[v_count].x = atof(words[1]);
            v[v_count].y = atof(words[2]);
            v[v_count].z = atof(words[3]);
            v_count++;
        }
        else if (strcmp(words[0], "vt") == 0) // texture
        {
            vt[vt_count].x = atof(words[1]);
            vt[vt_count].y = atof(words[2]);
            vt_count++;
        }
        else if (strcmp(words[0], "vn") == 0) // normal
        {
            vn[vn_count].x = atof(words[1]);
            vn[vn_count].y = atof(words[2]);
            vn[vn_count].z = atof(words[3]);
            vn_count++;
        }
        else if (strcmp(words[0], "f") == 0) // face
        {
            char *v1[3];
            char *v2[3];
            char *v3[3];

            v1[0] = strtok(words[1], "/");
            v1[1] = strtok(NULL, "/");
            v1[2] = strtok(NULL, "/");
            v2[0] = strtok(words[2], "/");
            v2[1] = strtok(NULL, "/");
            v2[2] = strtok(NULL, "/");
            v3[0] = strtok(words[3], "/");
            v3[1] = strtok(NULL, "/");
            v3[2] = strtok(NULL, "/");

            processVertex(vertices, v1, v, vt, vn);
            processVertex(vertices, v2, v, vt, vn);
            processVertex(vertices, v3, v, vt, vn);

            // push(triangles, );
            // tri_count++;
        }
    }

    fclose(fp); // close file
    if (line)   // clean up
    {
        free(line);
    }

    // for (int i = 0; i < vertices->size; i++)
    // {
    //     printf("%d", vertices[i]);
    // }

    return vertices;
    // verticesDest = vertices;
    // trianglesDest = triangles;
}

void processVertex(DynamicArray *vertices, char *vertexData[3], Vertex v[], Vertex vt[], Vertex vn[])
{
    int vertex_ptr = atoi(vertexData[0]) - 1; // ASCII to integer
    int texture_ptr = atoi(vertexData[1]) - 1;
    int normal_ptr = atoi(vertexData[2]) - 1;

    push(vertices, v[vertex_ptr].x);
    push(vertices, v[vertex_ptr].y);
    push(vertices, v[vertex_ptr].z);

    push(vertices, vn[normal_ptr].x);
    push(vertices, vn[normal_ptr].y);
    push(vertices, vn[normal_ptr].z);

    push(vertices, vt[texture_ptr].x);
    push(vertices, vt[texture_ptr].y);
}