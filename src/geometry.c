// #include "model.h"
// #include "geometry.h"

// #include <cglm/cglm.h>

// void laplace_beltrami_flow(Mesh *mesh)
// {
//     float **L = (float **)malloc(mesh->vertCount * sizeof(float *));
//     for (int i = 0; i < mesh->vertCount; i++)
//     {
//         L[i] = (float *)malloc(mesh->vertCount * sizeof(float));
//     }

//     // computeLaplacianMatrix(mesh->vertCount, );

//     applyLaplacian(mesh->vertCount, mesh->vertices, L, 0.01f);

//     // Clean up
//     for (int i = 0; i < mesh->vertCount; i++)
//     {
//         free(L[i]);
//     }
//     free(L);
// }

// float cotangent(vec3 v1, vec3 v2, vec3 v3)
// {
//     vec3 u, v;
//     glm_vec3_sub(v2, v1, u);
//     glm_vec3_sub(v3, v1, v);
//     float dot = glm_vec3_dot(u, v);
//     vec3 cross;
//     glm_vec3_cross(u, v, cross);
//     float cross_len = glm_vec3_norm(cross);
//     return dot / cross_len;
// }

// void computeLaplacianMatrix(int num_vertices, int num_triangles, vec3 *vertices, Triangle *triangles, float **L)
// {
//     // Initialize the Laplacian matrix with zeros
//     for (int i = 0; i < num_vertices; i++)
//     {
//         for (int j = 0; j < num_vertices; j++)
//         {
//             L[i][j] = 0.0f;
//         }
//     }

//     // Compute cotangent weights and populate the Laplacian matrix
//     for (int t = 0; t < num_triangles; t++)
//     {
//         int v1 = triangles[t].v1;
//         int v2 = triangles[t].v2;
//         int v3 = triangles[t].v3;

//         float cot1 = cotangent(vertices[v1], vertices[v2], vertices[v3]);
//         float cot2 = cotangent(vertices[v2], vertices[v3], vertices[v1]);
//         float cot3 = cotangent(vertices[v3], vertices[v1], vertices[v2]);

//         L[v1][v2] -= cot1 / 2.0f;
//         L[v2][v1] -= cot1 / 2.0f;

//         L[v2][v3] -= cot2 / 2.0f;
//         L[v3][v2] -= cot2 / 2.0f;

//         L[v3][v1] -= cot3 / 2.0f;
//         L[v1][v3] -= cot3 / 2.0f;

//         L[v1][v1] += cot1 / 2.0f;
//         L[v2][v2] += cot2 / 2.0f;
//         L[v3][v3] += cot3 / 2.0f;
//     }
// }

// void applyLaplacian(int num_vertices, vec3 *vertices, float **L, float delta_t)
// {
//     vec3 *new_positions = (vec3 *)malloc(num_vertices * sizeof(vec3));

//     for (int i = 0; i < num_vertices; i++)
//     {
//         vec3 laplacian = {0.0f, 0.0f, 0.0f};
//         for (int j = 0; j < num_vertices; j++)
//         {
//             vec3 temp;
//             glm_vec3_scale(vertices[j], L[i][j], temp);
//             glm_vec3_add(laplacian, temp, laplacian);
//         }
//         glm_vec3_scale(laplacian, delta_t, laplacian);
//         glm_vec3_add(vertices[i], laplacian, new_positions[i]);
//     }

//     // Update vertex positions
//     for (int i = 0; i < num_vertices; i++)
//     {
//         glm_vec3_copy(new_positions[i], vertices[i]);
//     }

//     free(new_positions);
// }