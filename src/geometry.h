#include "model.h"

typedef struct
{
    int v1, v2, v3; // Indices of the vertices in the triangle
} Triangle;

void laplace_beltrami_flow(Mesh *mesh);
float cotangent(vec3 v1, vec3 v2, vec3 v3);
void applyLaplacian(int num_vertices, vec3 *vertices, float **L, float delta_t);