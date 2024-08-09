#ifndef MODEL_H
#define MODEL_H

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#include <cglm/cglm.h>

// Vertex management settings
#define VERTEX_LIMIT 50000

// Model init settings
#define INIT_MODEL_POSITION \
    (vec3) { 0.0f, 0.0f, 0.0f }
#define INIT_ROTATION \
    (vec3) { 0.0f, 0.0f, 0.0f }
#define INIT_SCALE \
    (vec3) { 1.0f, 1.0f, 1.0f }

/*
 * Structs
 */

typedef struct
{
    vec3 position;  // vertex's position
    vec3 normal;    // vertex's normal
    vec3 curvature; // discrete analogue to curvature (or sometimes vector of flow movement)
} Vertex;

typedef struct
{
    Vertex *vertices;               // vertices of obj
    uint32_t *indices;              // indices of vertices
    GLuint VAO, VBO, IBO;           // buffers
    size_t numIndices, numVertices; // geometry stats
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
 * @return Initialized model.
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
 * @return Initialized mesh.
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

/**
 * @brief Initializes curvature of mesh.
 *
 * @param mesh Mesh to initialized curvature of.
 */
void initCurvature(Mesh *mesh);

#endif