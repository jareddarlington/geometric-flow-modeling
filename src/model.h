#ifndef MODEL_H
#define MODEL_H

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#include <cglm/cglm.h>

#include "utils.h"

typedef struct
{
    int vertCount;   // number of vertices
    float *vertices; // vertices
    GLuint VAO;      // vertex array object
    GLuint VBO;      // vertex buffer object
    GLuint posVBO;   // position VBO
    GLuint velVBO;   // velocity VBO
} Mesh;

typedef struct
{
    Mesh *mesh;          // mesh
    vec3 position;       // position
    vec3 rotation;       // rotation
    float scale;         // scale
    GLuint renderMethod; // render method
} Model;

typedef struct
{
    float x, y, z;
} Vertex;

Model *createModel(Mesh *mesh);
void destroyModel(Model *model);
Mesh *createMesh(const char *filename);
void destroyMesh(Mesh *mesh);
DynamicArray *loadOBJ(const char *filename);
void processVertex(DynamicArray *vertices, char *vertexData[3], Vertex v[], Vertex vt[], Vertex vn[]);

#endif