#ifndef MODEL_H
#define MODEL_H

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#include <cglm/cglm.h>

#include "utils.h"

// Vertex management settings
#define VERTEX_LIMIT 2000
#define STRIDE 6

// Model init settings
#define INIT_MODEL_POSITION \
    (vec3) { 0.0f, 0.0f, 0.0f }
#define INIT_ROTATION \
    (vec3) { 0.0f, -M_PI, 0.0f }
#define INIT_SCALE \
    (vec3) { 1.0f, 1.0f, 1.0f }

/*
 * Structures
 */

typedef struct
{
    int vertCount;   // number of vertices
    float *vertices; // vertices
    GLuint VAO;      // vertex array object
    GLuint VBO;      // vertex buffer object
} Mesh;

typedef struct
{
    Mesh *mesh;          // mesh
    vec3 position;       // position
    vec3 rotation;       // rotation
    vec3 scale;          // scale
    GLuint renderMethod; // render method
} Model;

typedef struct
{
    float x, y, z;
} Vertex;

/*
 * Function Prototypes
 */

/**
 * @brief Creates model and sets defaults.
 *
 * @param mesh Model's mesh.
 */
Model *createModel(Mesh *mesh);

/**
 * @brief Destroys model and frees space.
 *
 * @param model Model to destroy.
 */
void destroyModel(Model *model);

/**
 * @brief Computes model matrix for use in MVP.
 *
 * @param model Model to generate matrix for.
 * @param dest  Destination of model matrix.
 */
void computeModelMatrix(Model *model, mat4 *dest);

/**
 * @brief Creates mesh and binds buffers.
 *
 * @param filename .obj filename.
 */
Mesh *createMesh(const char *filename);

/**
 * @brief Destroys mesh and frees space.
 *
 * @param mesh Mesh to destroy.
 */
void destroyMesh(Mesh *mesh);

/**
 * @brief Parses and loads object.
 *
 * @param .obj filename.
 */
DynamicArray *loadOBJ(const char *filename);

/**
 * @brief Adds vertex data to dynamic arry.
 *
 * @param vertices   Dynamic Array to add to.
 * @param vertexData Vertex data.
 * @param v          Vertices.
 * @param vn         Normals of vertices.
 */
void processVertex(DynamicArray *vertices, char *vertexData[3], Vertex v[], Vertex vn[]);

#endif