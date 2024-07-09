// #include "graphics.h"

// void drawMesh(Mesh *mesh, GLuint shaderID, GLenum mode, vec3 *position, vec3 *rotation, float scale)
// {
//     vec3 scaling = {scale, scale, scale};

//     struct
//     {
//         mat4 position;
//         mat4 rotation;
//         mat4 scaling;
//         mat4 model;
//     } matrices;

//     // Position
//     glm_mat4_identity(matrices.position);
//     mat4_translation(matrices.position, matrices.position, position);

//     // Rotation
//     glm_mat4_identity(matrices.rotation);
//     mat4_from_eulers(matrices.rotation, rotation);

//     // Scaling
//     glm_mat4_identity(matrices.scaling);
//     mat4_scaling(matrices.scaling, matrices.scaling, scaling);
// }