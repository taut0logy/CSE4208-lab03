#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

unsigned int cubeVAO = 0, cubeVBO = 0;

// Cube vertices: position (3) + normal (3) + texcoord (2) = 8 floats per vertex
// Each face maps UV (0,0)→(1,1). Tiling is controlled by a texRepeat uniform.
float cubeVertices[] = {
    // positions          // normals            // texcoords
    // Back face  (z = 0)
    0.0f, 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
    0.0f, 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
    // Front face (z = 1)
    0.0f, 0.0f, 1.0f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,    0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
    // Left face  (x = 0)
    0.0f, 1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
    0.0f, 1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
    0.0f, 0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
    // Right face (x = 1)
    1.0f, 1.0f, 1.0f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    // Bottom face (y = 0)
    0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
    0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
    // Top face (y = 1)
    0.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
    1.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
    0.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
    0.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
};

void initCubeBuffers() {
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    int stride = 8 * sizeof(float);
    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texcoord attribute (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void drawCuboid(Shader& shader, glm::mat4 parentTrans,
                float posX, float posY, float posZ,
                float scaleX, float scaleY, float scaleZ,
                glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                float shininess = 32.0f, glm::vec3 emissive = glm::vec3(0.0f)) {
    shader.use();
    
    shader.setVec3("material.ambient", ambient);
    shader.setVec3("material.diffuse", diffuse);
    shader.setVec3("material.specular", specular);
    shader.setVec3("material.emissive", emissive);
    shader.setFloat("material.shininess", shininess);

    glm::mat4 model = glm::translate(parentTrans, glm::vec3(posX, posY, posZ));
    model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));
    shader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void drawCuboidSimple(Shader& shader, glm::mat4 parentTrans,
                      float posX, float posY, float posZ,
                      float scaleX, float scaleY, float scaleZ,
                      glm::vec3 color, float shininess = 32.0f, glm::vec3 emissive = glm::vec3(0.0f)) {
    drawCuboid(shader, parentTrans, posX, posY, posZ, scaleX, scaleY, scaleZ,
               color * 0.3f, color, glm::vec3(0.5f), shininess, emissive);
}

void cleanupCubeBuffers() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
}

#endif
