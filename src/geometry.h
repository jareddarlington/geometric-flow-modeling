#include "model.h"

// TODO: Add doc comments

void laplace_beltrami_flow(Mesh *mesh);
float cotangent(vec3 v1, vec3 v2, vec3 v3);
void computeLaplacianMatrix(int num_vertices, int fCount, vec3 *vertices, Face *faces, float **L);
void applyLaplacian(int num_vertices, vec3 *vertices, float **L, float delta_t);