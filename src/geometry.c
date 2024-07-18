#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <math.h>

#include "geometry.h"
#include "model.h"

void computeGeometry(GLFWwindow *window, Model *model, GEOMETRIC_FLOW flow)
{
    if (flow == MCF)
        meanCurvatureFlow(model->mesh);

    glBindBuffer(GL_ARRAY_BUFFER, model->mesh->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, model->mesh->numVertices * sizeof(Vertex), model->mesh->vertices);
}

void meanCurvatureFlow(Mesh *mesh)
{
    computeNormals(mesh);
    vec3 *H;
    computeMeanCurvature(mesh, H);
}

void computeNormals(Mesh *mesh)
{
    // Set all vertex normals to 0
    for (size_t i = 0; i < mesh->numVertices; i++)
        glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, mesh->vertices[i].normal);

    for (size_t i = 0; i < mesh->numIndices / 3; i++)
    {
        // Grab face indices
        int v1Index = mesh->indices[3 * i];
        int v2Index = mesh->indices[3 * i + 1];
        int v3Index = mesh->indices[3 * i + 2];

        // Grab vertices in face
        vec3 v1, v2, v3;
        glm_vec3_copy(mesh->vertices[v1Index].position, v1);
        glm_vec3_copy(mesh->vertices[v2Index].position, v2);
        glm_vec3_copy(mesh->vertices[v3Index].position, v3);

        // Calculate face edges
        vec3 e1, e2;
        glm_vec3_sub(v2, v1, e1);
        glm_vec3_sub(v3, v1, e2);

        // Calculate face normal
        vec3 faceNormal;
        glm_vec3_crossn(e1, e2, faceNormal);

        // Add face normal to vertices of face
        glm_vec3_add(mesh->vertices[v1Index].normal, faceNormal, mesh->vertices[v1Index].normal);
        glm_vec3_add(mesh->vertices[v2Index].normal, faceNormal, mesh->vertices[v2Index].normal);
        glm_vec3_add(mesh->vertices[v3Index].normal, faceNormal, mesh->vertices[v3Index].normal);
    }

    // Normalize sums
    for (size_t i = 0; i < mesh->numVertices; i++)
        glm_vec3_normalize(mesh->vertices[i].normal);
}

void computeMeanCurvature(Mesh *mesh, vec3 *dest)
{
    // Initalize mean curvatures and voronoi areas
    vec3 H[mesh->numVertices];
    float areas[mesh->numVertices];
    for (size_t i = 0; i < mesh->numVertices; i++)
    {
        glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, H[i]);
        areas[i] = 0.0f;
    }

    // Compute mean curvatures
    for (size_t i = 0; i < mesh->numIndices / 3; i++)
    {
        // Grab face indices
        int v1Index = mesh->indices[3 * i];
        int v2Index = mesh->indices[3 * i + 1];
        int v3Index = mesh->indices[3 * i + 2];

        // Grab vertices in face
        vec3 v1, v2, v3;
        glm_vec3_copy(mesh->vertices[v1Index].position, v1);
        glm_vec3_copy(mesh->vertices[v2Index].position, v2);
        glm_vec3_copy(mesh->vertices[v3Index].position, v3);

        // Calculate angles opposite to edges
        float alpha = computeOppositeAngle(v2, v3, v1);
        float beta = computeOppositeAngle(v1, v3, v2);
        float gamma = computeOppositeAngle(v1, v2, v3);

        // Calculate cotangent weights
        float cotAlpha = 1 / tan(alpha);
        float cotBeta = 1 / tan(beta);
        float cotGamma = 1 / tan(gamma);

        // Update mean curvatures
        vec3 temp;

        glm_vec3_sub(mesh->vertices[v2Index].position, mesh->vertices[v1Index].position, temp);
        glm_vec3_scale(temp, cotBeta + cotGamma, temp);
        glm_vec3_add(H[v1Index], temp, H[v1Index]);

        glm_vec3_sub(mesh->vertices[v3Index].position, mesh->vertices[v2Index].position, temp);
        glm_vec3_scale(temp, cotAlpha + cotGamma, temp);
        glm_vec3_add(H[v2Index], temp, H[v2Index]);

        glm_vec3_sub(mesh->vertices[v1Index].position, mesh->vertices[v3Index].position, temp);
        glm_vec3_scale(temp, cotAlpha + cotBeta, temp);
        glm_vec3_add(H[v3Index], temp, H[v3Index]);

        // Update voronoi areas
        vec3 faceNormal;
        computeFaceNormal(v1, v2, v3, faceNormal);
        float faceArea = glm_vec3_norm(faceNormal) / 2;

        areas[v1Index] += faceArea / 3;
        areas[v2Index] += faceArea / 3;
        areas[v3Index] += faceArea / 3;
    }

    // Normalize mean curvature by area
    for (size_t i = 0; i < mesh->numVertices; i++)
        glm_vec3_divs(H[i], 2 * areas[i], H[i]);

    dest = H;
}

float computeOppositeAngle(vec3 v1, vec3 v2, vec3 v3)
{
    vec3 e1, e2;
    glm_vec3_sub(v2, v1, e1);
    glm_vec3_sub(v3, v1, e2);
    return acos(glm_vec3_dot(e1, e2) / glm_vec3_norm(e1) * glm_vec3_norm(e2));
}

void computeFaceNormal(vec3 v1, vec3 v2, vec3 v3, vec3 *dest)
{
    // Calculate face edges
    vec3 e1, e2;
    glm_vec3_sub(v2, v1, e1);
    glm_vec3_sub(v3, v1, e2);

    // Calculate face normal
    vec3 faceNormal;
    glm_vec3_crossn(e1, e2, faceNormal);

    // Copy over normal
    glm_vec3_copy(faceNormal, dest);
}