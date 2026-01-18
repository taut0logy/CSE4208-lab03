#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    
    float pitch, yaw, roll;
    float moveSpeed, rotateSpeed;
    float orbitRadius;
    float orbitAngle;
    
    Camera(glm::vec3 pos = glm::vec3(0.0f, 10.0f, 0.0f), 
           glm::vec3 tgt = glm::vec3(0.0f, 0.0f, 0.0f)) {
        position = pos;
        target = tgt;
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        pitch = 0.0f;
        yaw = -90.0f;
        roll = 0.0f;
        moveSpeed = 5.0f;
        rotateSpeed = 50.0f;
        orbitRadius = glm::length(position - target);
        orbitAngle = 0.0f;
    }
    
    glm::mat4 getViewMatrix() {
        glm::mat4 view = glm::lookAt(position, target, up);
        view = glm::rotate(view, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
        return view;
    }
    
    void moveForward(float deltaTime) {
        glm::vec3 direction = glm::normalize(target - position);
        position += direction * moveSpeed * deltaTime;
        target += direction * moveSpeed * deltaTime;
    }
    
    void moveBackward(float deltaTime) {
        glm::vec3 direction = glm::normalize(target - position);
        position -= direction * moveSpeed * deltaTime;
        target -= direction * moveSpeed * deltaTime;
    }
    
    void moveLeft(float deltaTime) {
        glm::vec3 direction = glm::normalize(target - position);
        glm::vec3 right = glm::normalize(glm::cross(direction, up));
        position -= right * moveSpeed * deltaTime;
        target -= right * moveSpeed * deltaTime;
    }
    
    void moveRight(float deltaTime) {
        glm::vec3 direction = glm::normalize(target - position);
        glm::vec3 right = glm::normalize(glm::cross(direction, up));
        position += right * moveSpeed * deltaTime;
        target += right * moveSpeed * deltaTime;
    }
    
    void moveUp(float deltaTime) {
        position.y += moveSpeed * deltaTime;
        target.y += moveSpeed * deltaTime;
    }
    
    void moveDown(float deltaTime) {
        position.y -= moveSpeed * deltaTime;
        target.y -= moveSpeed * deltaTime;
    }
    
    void addPitch(float deltaTime) {
        pitch += rotateSpeed * deltaTime;
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        updateTarget();
    }
    
    void addYaw(float deltaTime) {
        yaw += rotateSpeed * deltaTime;
        updateTarget();
    }
    
    void addRoll(float deltaTime) {
        roll += rotateSpeed * deltaTime;
    }
    
    void orbit(float deltaTime) {
        orbitAngle += rotateSpeed * deltaTime;
        position.x = target.x + orbitRadius * cos(glm::radians(orbitAngle));
        position.z = target.z + orbitRadius * sin(glm::radians(orbitAngle));
    }
    
    void setBirdsEyeView(float labCenterX, float labCenterZ) {
        target = glm::vec3(labCenterX, 0.0f, labCenterZ);
        position = glm::vec3(labCenterX - 5.0f, 15.0f, labCenterZ + 12.0f);
        pitch = 0.0f;
        yaw = -90.0f;
        roll = 0.0f;
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        orbitRadius = glm::length(position - target);
    }
    
    void resetView(float labCenterX, float labCenterZ, float labDepth) {
        target = glm::vec3(labCenterX, 1.5f, labCenterZ);
        position = glm::vec3(labCenterX - 5.0f, 15.0f, labCenterZ + 12.0f);
        pitch = 0.0f;
        yaw = -90.0f;
        roll = 0.0f;
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        orbitRadius = glm::length(position - target);
    }

    // Processes input received from a mouse input system
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= 0.1f; // Sensitivity
        yoffset *= 0.1f;

        yaw   += xoffset;
        pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (pitch > 89.0f)  pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        updateTarget();
    }

    // Processes input received from a mouse scroll-wheel event
    void ProcessMouseScroll(float yoffset) {
        moveForward(yoffset * 0.5f);
    }
    
private:
    void updateTarget() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        target = position + glm::normalize(direction);
    }
};

#endif
