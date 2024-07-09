#ifndef MODEL_H
#define MODEL_H

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#include <cglm/cglm.h>

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
Mesh *createMesh(const char *filename, bool instanced);
void destroyMesh(Mesh *mesh);

#endif