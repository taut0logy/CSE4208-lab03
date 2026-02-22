#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "shader.h"

class SpotLight
{
public:
    int       lightNumber;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float k_c, k_l, k_q;
    float cutOff;       // cos of inner angle
    float outerCutOff;  // cos of outer angle

    SpotLight(int num,
              glm::vec3 pos, glm::vec3 dir,
              glm::vec3 amb, glm::vec3 diff, glm::vec3 spec,
              float constant, float linear, float quadratic,
              float innerDeg, float outerDeg)
    {
        lightNumber = num;
        position    = pos;
        direction   = glm::normalize(dir);
        ambient     = amb;  diffuse = diff;  specular = spec;
        k_c = constant;  k_l = linear;  k_q = quadratic;
        cutOff      = glm::cos(glm::radians(innerDeg));
        outerCutOff = glm::cos(glm::radians(outerDeg));
    }

    void setUpLight(Shader& shader)
    {
        shader.use();
        std::string b = "spotLights[" + std::to_string(lightNumber) + "].";
        shader.setVec3 (b + "position",    position);
        shader.setVec3 (b + "direction",   direction);
        shader.setVec3 (b + "ambient",     ambientOn  * ambient);
        shader.setVec3 (b + "diffuse",     diffuseOn  * diffuse);
        shader.setVec3 (b + "specular",    specularOn * specular);
        shader.setFloat(b + "k_c",         k_c);
        shader.setFloat(b + "k_l",         k_l);
        shader.setFloat(b + "k_q",         k_q);
        shader.setFloat(b + "cutOff",      cutOff);
        shader.setFloat(b + "outerCutOff", outerCutOff);
    }

    void turnOff() { ambientOn = diffuseOn = specularOn = 0.0f; }
    void turnOn()  { ambientOn = diffuseOn = specularOn = 1.0f; }
    void setAmbient (bool on) { ambientOn  = on ? 1.0f : 0.0f; }
    void setDiffuse (bool on) { diffuseOn  = on ? 1.0f : 0.0f; }
    void setSpecular(bool on) { specularOn = on ? 1.0f : 0.0f; }

private:
    float ambientOn = 1.0f, diffuseOn = 1.0f, specularOn = 1.0f;
};

#endif
