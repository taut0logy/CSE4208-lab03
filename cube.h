#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

unsigned int cubeVAO = 0, cubeVBO = 0, cubeEBO = 0;

void initCubeBuffers() {
    if (cubeVAO != 0) return;
    
    float vertices[] = {
        // positions          
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f
    };
    
    unsigned int indices[] = {
        0, 3, 2, 2, 1, 0,  // front
        1, 2, 6, 6, 5, 1,  // right
        5, 6, 7, 7, 4, 5,  // back
        4, 7, 3, 3, 0, 4,  // left
        3, 7, 6, 6, 2, 3,  // top
        4, 0, 1, 1, 5, 4   // bottom
    };
    
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);
    
    glBindVertexArray(cubeVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void drawCuboid(Shader& shader, glm::mat4 parentTrans, 
                float posX, float posY, float posZ,
                float scaleX, float scaleY, float scaleZ,
                glm::vec3 color) {
    shader.use();
    shader.setVec3("color", color);
    
    glm::mat4 model = glm::translate(parentTrans, glm::vec3(posX, posY, posZ));
    model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));
    shader.setMat4("model", model);
    
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawCuboidRotated(Shader& shader, glm::mat4 parentTrans,
                       float posX, float posY, float posZ,
                       float rotX, float rotY, float rotZ,
                       float scaleX, float scaleY, float scaleZ,
                       glm::vec3 color) {
    shader.use();
    shader.setVec3("color", color);
    
    glm::mat4 model = glm::translate(parentTrans, glm::vec3(posX, posY, posZ));
    model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));
    shader.setMat4("model", model);
    
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void cleanupCubeBuffers() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);
}

#endif
