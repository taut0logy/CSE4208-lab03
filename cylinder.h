#ifndef CYLINDER_H
#define CYLINDER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include "shader.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

unsigned int cylinderVAO = 0, cylinderVBO = 0, cylinderEBO = 0;
int cylinderIndexCount = 0;

// Generates a unit cylinder (radius=0.5, height=1) centered at base (0,0,0) to (0,1,0)
// Vertex format: pos(3) + normal(3) + texcoord(2) = 8 floats
void initCylinderBuffers(int sectors = 36)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float radius = 0.5f;
    float height = 1.0f;
    float sectorStep = 2.0f * (float)M_PI / sectors;

    // --- Side vertices ---
    // Two rings: bottom (y=0) and top (y=1)
    for (int i = 0; i <= sectors; i++)
    {
        float angle = i * sectorStep;
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        float nx = cosf(angle);
        float nz = sinf(angle);
        float u = (float)i / (float)sectors;

        // Bottom ring
        vertices.push_back(x + 0.5f);
        vertices.push_back(0.0f);
        vertices.push_back(z + 0.5f);
        vertices.push_back(nx);
        vertices.push_back(0.0f);
        vertices.push_back(nz);
        vertices.push_back(u);    // u
        vertices.push_back(0.0f); // v = bottom

        // Top ring
        vertices.push_back(x + 0.5f);
        vertices.push_back(height);
        vertices.push_back(z + 0.5f);
        vertices.push_back(nx);
        vertices.push_back(0.0f);
        vertices.push_back(nz);
        vertices.push_back(u);    // u
        vertices.push_back(1.0f); // v = top
    }

    // Side indices
    for (int i = 0; i < sectors; i++)
    {
        int bottom = i * 2;
        int top = i * 2 + 1;
        int nextBottom = (i + 1) * 2;
        int nextTop = (i + 1) * 2 + 1;

        indices.push_back(bottom);
        indices.push_back(nextBottom);
        indices.push_back(top);

        indices.push_back(top);
        indices.push_back(nextBottom);
        indices.push_back(nextTop);
    }

    // --- Bottom cap ---
    int bottomCenterIdx = (int)vertices.size() / 8;
    // Center vertex
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.5f); // u center
    vertices.push_back(0.5f); // v center

    int bottomRingStart = (int)vertices.size() / 8;
    for (int i = 0; i <= sectors; i++)
    {
        float angle = i * sectorStep;
        float x = radius * cosf(angle) + 0.5f;
        float z = radius * sinf(angle) + 0.5f;
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(z);
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);
        vertices.push_back(0.0f);
        vertices.push_back(x); // u = x position
        vertices.push_back(z); // v = z position
    }

    for (int i = 0; i < sectors; i++)
    {
        indices.push_back(bottomCenterIdx);
        indices.push_back(bottomRingStart + i + 1);
        indices.push_back(bottomRingStart + i);
    }

    // --- Top cap ---
    int topCenterIdx = (int)vertices.size() / 8;
    vertices.push_back(0.5f);
    vertices.push_back(height);
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.5f); // u center
    vertices.push_back(0.5f); // v center

    int topRingStart = (int)vertices.size() / 8;
    for (int i = 0; i <= sectors; i++)
    {
        float angle = i * sectorStep;
        float x = radius * cosf(angle) + 0.5f;
        float z = radius * sinf(angle) + 0.5f;
        vertices.push_back(x);
        vertices.push_back(height);
        vertices.push_back(z);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);
        vertices.push_back(x); // u = x position
        vertices.push_back(z); // v = z position
    }

    for (int i = 0; i < sectors; i++)
    {
        indices.push_back(topCenterIdx);
        indices.push_back(topRingStart + i);
        indices.push_back(topRingStart + i + 1);
    }

    cylinderIndexCount = (int)indices.size();

    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(1, &cylinderVBO);
    glGenBuffers(1, &cylinderEBO);

    glBindVertexArray(cylinderVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    int stride = 8 * sizeof(float);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texcoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

// Draw a cylinder with given position, scale, and material
void drawCylinder(Shader &shader, glm::mat4 parentTrans,
                  float posX, float posY, float posZ,
                  float scaleX, float scaleY, float scaleZ,
                  glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                  float shininess = 32.0f, glm::vec3 emissive = glm::vec3(0.0f))
{
    shader.use();

    shader.setVec3("material.ambient", ambient);
    shader.setVec3("material.diffuse", diffuse);
    shader.setVec3("material.specular", specular);
    shader.setVec3("material.emissive", emissive);
    shader.setFloat("material.shininess", shininess);

    glm::mat4 model = glm::translate(parentTrans, glm::vec3(posX, posY, posZ));
    model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));
    shader.setMat4("model", model);

    glBindVertexArray(cylinderVAO);
    glDrawElements(GL_TRIANGLES, cylinderIndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Simplified cylinder draw (auto-compute ambient from color)
void drawCylinderSimple(Shader &shader, glm::mat4 parentTrans,
                        float posX, float posY, float posZ,
                        float scaleX, float scaleY, float scaleZ,
                        glm::vec3 color, float shininess = 32.0f, glm::vec3 emissive = glm::vec3(0.0f))
{
    drawCylinder(shader, parentTrans, posX, posY, posZ, scaleX, scaleY, scaleZ,
                 color * 0.3f, color, glm::vec3(0.5f), shininess, emissive);
}

void cleanupCylinderBuffers()
{
    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteBuffers(1, &cylinderVBO);
    glDeleteBuffers(1, &cylinderEBO);
}

#endif
