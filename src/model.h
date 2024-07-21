#ifndef MODEL_H
#define MODEL_H

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#include <cglm/cglm.h>

#include "utils.h"

// Vertex management settings
#define VERTEX_LIMIT 5000

// Model init settings
#define INIT_MODEL_POSITION \
    (vec3) { 0.0f, 0.0f, 0.0f }
#define INIT_ROTATION \
    (vec3) { 0.0f, 0.0f, 0.0f }
#define INIT_SCALE \
    (vec3) { 1.0f, 1.0f, 1.0f }

/*
 * Structures
 */

typedef struct
{
    vec3 position;  // vertex's position
    vec3 normal;    // vertex's normal
    vec3 curvature; // discrete analogue to curvature or vector of flow movement
} Vertex;

typedef struct
{
    Vertex *vertices;
    uint32_t *indices;
    GLuint VAO, VBO, IBO;
    size_t numIndices, numVertices;
} Mesh;

typedef struct
{
    Mesh *mesh;          // mesh
    vec3 position;       // position
    vec3 rotation;       // rotation
    vec3 scale;          // scale
    GLuint renderMethod; // render method
} Model;

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
 * @brief Computes model matrix for use in MVP.
 *
 * @param model Model to generate matrix for.
 * @param dest  Destination of model matrix.
 */
void computeModelMatrix(Model *model, mat4 *dest);

/**
 * @brief Loads .obj file into mesh.
 *
 * @param filename Name of file to load.
 * @param mesh     Mesh to load data into.
 */
void loadOBJ(const char *filename, Mesh *mesh);

#endif