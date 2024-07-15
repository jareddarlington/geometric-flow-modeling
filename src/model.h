#ifndef MODEL_H
#define MODEL_H

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#include <cglm/cglm.h>

#include "utils.h"

// Vertex management settings
#define VERTEX_LIMIT 2000

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
    vec3 position;
    vec3 normal;
} Vertex;

typedef struct
{
    Vertex *vertices;      // vertices
    unsigned int *indices; // indices
    GLuint VAO;            // vertex array object
    GLuint VBO;            // vertex buffer object
    GLuint IBO;            // index buffer object
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

void fakeLoad(Mesh *mesh);

#endif