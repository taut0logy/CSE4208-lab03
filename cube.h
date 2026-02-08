#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

unsigned int cubeVAO = 0, cubeVBO = 0;

// Cube vertices with positions and normals
float cubeVertices[] = {
    // positions          // normals
    // Back face
    0.0f, 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
    1.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
    1.0f, 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
    1.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
    0.0f, 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
    0.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
    // Front face
    0.0f, 0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
    1.0f, 0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
    1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
    1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
    0.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
    0.0f, 0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
    // Left face
    0.0f, 1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
    0.0f, 1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
    0.0f, 0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
    0.0f, 0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
    0.0f, 0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
    0.0f, 1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
    // Right face
    1.0f, 1.0f, 1.0f,    1.0f,  0.0f,  0.0f,
    1.0f, 0.0f, 0.0f,    1.0f,  0.0f,  0.0f,
    1.0f, 1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
    1.0f, 0.0f, 0.0f,    1.0f,  0.0f,  0.0f,
    1.0f, 1.0f, 1.0f,    1.0f,  0.0f,  0.0f,
    1.0f, 0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
    // Bottom face
    0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
    1.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
    1.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
    1.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
    0.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
    0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
    // Top face
    0.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
    1.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,
    1.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
    1.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,
    0.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
    0.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,
};

void initCubeBuffers() {
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
