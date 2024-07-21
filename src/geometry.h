#include "model.h"

// TODO: update docs

typedef enum
{
    MCF_VBM, // mean curvature flow (vertex-based method)
    MCF_ITI  // mean curvature flow (implicit time integration)
} GEOMETRIC_FLOW;

void computeGeometry(GLFWwindow *window, Model *model, GEOMETRIC_FLOW flow, bool flowing);
void mcfVBM(Mesh *mesh);
void mcfITI(Mesh *mesh);
void computeNormals(Mesh *mesh);