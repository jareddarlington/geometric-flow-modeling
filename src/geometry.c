#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <math.h>

#include "geometry.h"
#include "model.h"

void computeGeometry(GLFWwindow *window, Model *model, GEOMETRIC_FLOW flow, bool flowing)
{
    // Calculate change in time
    static double lastTime = 0.0;
    if (lastTime == 0.0)
        lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - lastTime);

    // Compute flow if enabled
    if (flowing)
    {
        if (flow == MCF_VBM)
            mcfVBM(model->mesh, deltaTime);
        else if (flow == MCF_ITI)
            mcfITI(model->mesh, deltaTime);
    }

    // Rebind and upload new geometry
    glBindBuffer(GL_ARRAY_BUFFER, model->mesh->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, model->mesh->numVertices * sizeof(Vertex), model->mesh->vertices);

    lastTime = currentTime;
}

void mcfVBM(Mesh *mesh, float deltaTime)
{
    // Reset all curvature to 0
    for (size_t i = 0; i < mesh->numVertices; i++)
        glm_vec3_zero(mesh->vertices[i].curvature);

    // Calculate curvature
    for (size_t i = 0; i < mesh->numIndices; i += 3)
    {
        uint32_t v0 = mesh->indices[i];
        uint32_t v1 = mesh->indices[i + 1];
        uint32_t v2 = mesh->indices[i + 2];

        vec3 diff;
        glm_vec3_sub(mesh->vertices[v1].position, mesh->vertices[v0].position, diff);
        glm_vec3_add(mesh->vertices[v0].curvature, diff, mesh->vertices[v0].curvature);
        glm_vec3_sub(mesh->vertices[v0].position, mesh->vertices[v1].position, diff);
        glm_vec3_add(mesh->vertices[v1].curvature, diff, mesh->vertices[v1].curvature);

        glm_vec3_sub(mesh->vertices[v2].position, mesh->vertices[v1].position, diff);
        glm_vec3_add(mesh->vertices[v1].curvature, diff, mesh->vertices[v1].curvature);
        glm_vec3_sub(mesh->vertices[v1].position, mesh->vertices[v2].position, diff);
        glm_vec3_add(mesh->vertices[v2].curvature, diff, mesh->vertices[v2].curvature);

        glm_vec3_sub(mesh->vertices[v0].position, mesh->vertices[v2].position, diff);
        glm_vec3_add(mesh->vertices[v2].curvature, diff, mesh->vertices[v2].curvature);
        glm_vec3_sub(mesh->vertices[v2].position, mesh->vertices[v0].position, diff);
        glm_vec3_add(mesh->vertices[v0].curvature, diff, mesh->vertices[v0].curvature);
    }

    for (size_t i = 0; i < mesh->numVertices; i++)
    {
        // Update positions based on curvature
        vec3 update;
        glm_vec3_scale(mesh->vertices[i].curvature, deltaTime * 10, update);
        glm_vec3_add(mesh->vertices[i].position, update, mesh->vertices[i].position);

        // Scale curvature for heat map coloring
        glm_vec3_scale(mesh->vertices[i].curvature, 100.0f, mesh->vertices[i].curvature);
    }
}

void mcfITI(Mesh *mesh, float deltaTime)
{
}

void computeNormals(Mesh *mesh)
{
    // Reset all normals
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