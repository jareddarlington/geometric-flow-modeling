#include "model.h"

typedef enum
{
    MCF // mean curvature flow
} GEOMETRIC_FLOW;

void computeGeometry(GLFWwindow *window, Model *model, GEOMETRIC_FLOW flow, bool flowing);
void meanCurvatureFlow(Mesh *mesh);
void computeNormals(Mesh *mesh);
void computeMeanCurvature(Mesh *mesh);
float computeOppositeAngle(vec3 v1, vec3 v2, vec3 v3);