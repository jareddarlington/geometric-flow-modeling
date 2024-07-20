#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <math.h>

#include "geometry.h"
#include "model.h"

void computeGeometry(GLFWwindow *window, Model *model, GEOMETRIC_FLOW flow, bool flowing)
{
    if (flowing)
    {
        if (flow == MCF)
        {
            meanCurvatureFlow(model->mesh);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, model->mesh->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, model->mesh->numVertices * sizeof(Vertex), model->mesh->vertices);
}

void meanCurvatureFlow(Mesh *mesh)
{
    static double lastTime = 0.0;
    if (lastTime == 0.0)
        lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - lastTime);

    computeMeanCurvature(mesh);
    for (size_t i = 0; i < mesh->numVertices; i++)
    {
        vec3 temp;
        glm_vec3_scale(mesh->vertices[i].curvature, deltaTime * mesh->numVertices, temp);
        glm_vec3_add(mesh->vertices[i].position, temp, mesh->vertices[i].position);
        // printf("%f %f %f\n", mesh-   >vertices[i].curvature[0], mesh->vertices[i].curvature[1], mesh->vertices[i].curvature[2]);
    }
    // printf("\n");

    lastTime = currentTime;
}

void computeNormals(Mesh *mesh)
{
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

void computeMeanCurvature(Mesh *mesh)
{
    // Initalize mean curvatures and voronoi areas
    float areas[mesh->numVertices];
    for (size_t i = 0; i < mesh->numVertices; i++)
    {
        // glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, mesh->vertices[i].curvature);
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

        // Update curvatures
        vec3 temp;

        // v1 curvature
        glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, temp);
        glm_vec3_sub(mesh->vertices[v2Index].position, mesh->vertices[v1Index].position, temp);
        glm_vec3_scale(temp, cotBeta + cotGamma, temp);
        glm_vec3_add(mesh->vertices[v1Index].curvature, temp, mesh->vertices[v1Index].curvature);

        glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, temp);
        glm_vec3_sub(mesh->vertices[v3Index].position, mesh->vertices[v1Index].position, temp);
        glm_vec3_scale(temp, cotAlpha + cotGamma, temp);
        glm_vec3_add(mesh->vertices[v1Index].curvature, temp, mesh->vertices[v1Index].curvature);

        // v2 curvature
        glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, temp);
        glm_vec3_sub(mesh->vertices[v1Index].position, mesh->vertices[v2Index].position, temp);
        glm_vec3_scale(temp, cotAlpha + cotGamma, temp);
        glm_vec3_add(mesh->vertices[v2Index].curvature, temp, mesh->vertices[v2Index].curvature);

        glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, temp);
        glm_vec3_sub(mesh->vertices[v3Index].position, mesh->vertices[v2Index].position, temp);
        glm_vec3_scale(temp, cotBeta + cotAlpha, temp);
        glm_vec3_add(mesh->vertices[v2Index].curvature, temp, mesh->vertices[v2Index].curvature);

        // v3 curvature
        glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, temp);
        glm_vec3_sub(mesh->vertices[v1Index].position, mesh->vertices[v3Index].position, temp);
        glm_vec3_scale(temp, cotAlpha + cotBeta, temp);
        glm_vec3_add(mesh->vertices[v3Index].curvature, temp, mesh->vertices[v3Index].curvature);

        glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, temp);
        glm_vec3_sub(mesh->vertices[v2Index].position, mesh->vertices[v3Index].position, temp);
        glm_vec3_scale(temp, cotBeta + cotGamma, temp);
        glm_vec3_add(mesh->vertices[v3Index].curvature, temp, mesh->vertices[v3Index].curvature);

        // Calculate face edges
        vec3 e1, e2;
        glm_vec3_sub(v2, v1, e1);
        glm_vec3_sub(v3, v1, e2);

        // Calculate face normal
        vec3 faceNormal;
        glm_vec3_crossn(e1, e2, faceNormal);
        float faceArea = glm_vec3_norm(faceNormal) / 2;

        areas[v1Index] += faceArea / 3;
        areas[v2Index] += faceArea / 3;
        areas[v3Index] += faceArea / 3;
    }

    // Normalize mean curvature by area
    for (size_t i = 0; i < mesh->numVertices; i++)
        glm_vec3_divs(mesh->vertices[i].curvature, 2 * areas[i], mesh->vertices[i].curvature);
}

float computeOppositeAngle(vec3 v1, vec3 v2, vec3 v3)
{
    vec3 e1, e2;
    glm_vec3_sub(v2, v1, e1);
    glm_vec3_sub(v3, v1, e2);
    return acos(glm_vec3_dot(e1, e2) / glm_vec3_norm(e1) * glm_vec3_norm(e2));
}