#include "model.h"

typedef enum
{
    MCF // mean curvature flow
} GEOMETRIC_FLOW;

void computeGeometry(GLFWwindow *window, Model *model, GEOMETRIC_FLOW flow);
void meanCurvatureFlow(Mesh *mesh);
void computeNormals(Mesh *mesh);
void computeMeanCurvature(Mesh *mesh, vec3 *dest);
float computeOppositeAngle(vec3 v1, vec3 v2, vec3 v3);
void computeFaceNormal(vec3 v1, vec3 v2, vec3 v3, vec3 *dest);