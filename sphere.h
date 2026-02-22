#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include "shader.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

unsigned int sphereVAO = 0, sphereVBO = 0, sphereEBO = 0;
int sphereIndexCount = 0;

// Generates a unit sphere (radius=0.5) centered at (0.5, 0.5, 0.5)
// Vertex format: pos(3) + normal(3) + texcoord(2) = 8 floats
void initSphereBuffers(int sectors = 36, int stacks = 18)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float radius = 0.5f;
    float sectorStep = 2.0f * (float)M_PI / sectors;
    float stackStep = (float)M_PI / stacks;

    for (int i = 0; i <= stacks; i++)
    {
        float stackAngle = (float)M_PI / 2.0f - i * stackStep; // from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);
        float y = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; j++)
        {
            float sectorAngle = j * sectorStep;

            float x = xy * cosf(sectorAngle);
            float z = xy * sinf(sectorAngle);

            // Position (shifted to [0,1] range)
            vertices.push_back(x + 0.5f);
            vertices.push_back(y + 0.5f);
            vertices.push_back(z + 0.5f);

            // Normal (unit sphere normal = normalized position from center)
            float nx = cosf(stackAngle) * cosf(sectorAngle);
            float ny = sinf(stackAngle);
            float nz = cosf(stackAngle) * sinf(sectorAngle);
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

            // Texture coordinates (spherical mapping)
            float u = (float)j / (float)sectors;
            float v = (float)i / (float)stacks;
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    // Indices
    for (int i = 0; i < stacks; i++)
    {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; j++, k1++, k2++)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    sphereIndexCount = (int)indices.size();

    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
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

// Draw a sphere with full material control
void drawSphere(Shader &shader, glm::mat4 parentTrans,
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

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Simplified sphere draw
void drawSphereSimple(Shader &shader, glm::mat4 parentTrans,
                      float posX, float posY, float posZ,
                      float scaleX, float scaleY, float scaleZ,
                      glm::vec3 color, float shininess = 32.0f, glm::vec3 emissive = glm::vec3(0.0f))
{
    drawSphere(shader, parentTrans, posX, posY, posZ, scaleX, scaleY, scaleZ,
               color * 0.3f, color, glm::vec3(0.5f), shininess, emissive);
}

void cleanupSphereBuffers()
{
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteBuffers(1, &sphereVBO);
    glDeleteBuffers(1, &sphereEBO);
}

#endif
