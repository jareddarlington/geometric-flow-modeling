#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "model.h"

/*
 * Enums
 */

/**
 * @brief Available geometric flow types.
 */
typedef enum
{
    MCF_VBM, // mean curvature flow (vertex-based method)
    MCF_ITI  // mean curvature flow (implicit time integration)
} GEOMETRIC_FLOW;

/*
 * Function Prototypes
 */

/**
 * @brief Calls for new geometry to be computed and binds it.
 *
 * @param window  GLFW window.
 * @param model   Model to compute flow on.
 * @param flow    Type of flow to compute.
 * @param flowing Whether to compute new geometry or not.
 */
void computeGeometry(GLFWwindow *window, Model *model, GEOMETRIC_FLOW flow, bool flowing);

/**
 * @brief Computes mean curvature flow (vertex-based method) on given mesh.
 *
 * @param mesh      Mesh to compute flow on.
 * @param deltaTime Time since last update.
 */
void mcfVBM(Mesh *mesh, float deltaTime);

/**
 * @brief Computes mean curvature flow (implicit time integration) on given mesh.
 *
 * @param mesh      Mesh to compute flow on.
 * @param deltaTime Time since last update.
 */
void mcfITI(Mesh *mesh, float deltaTime);

/**
 * @brief Computes normals of given mesh.
 *
 * @param mesh Mesh to compute normals for.
 */
void computeNormals(Mesh *mesh);

#endif