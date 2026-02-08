#ifndef LAB_SCENE_H
#define LAB_SCENE_H

#include <glm/glm.hpp>
#include "shader.h"
#include "objects.h"

// Room dimensions
const float ROOM_WIDTH = 8.0f;
const float ROOM_HEIGHT = 3.5f;
const float ROOM_DEPTH = 12.0f;

// Scene state
struct SceneState {
    bool pointLightsOn = true;
    bool ambientOn = true;
    bool diffuseOn = true;
    bool specularOn = true;
    
    // Door/Window animation
    bool doorOpening = false;
    bool windowOpening = false;
    float doorOpenAmount = 0.0f;
    float windowOpenAmount = 0.0f;
};

void updateSceneState(SceneState& state, float deltaTime) {
    float speed = 1.5f;
    if (state.doorOpening && state.doorOpenAmount < 1.0f)
        state.doorOpenAmount += speed * deltaTime;
    else if (!state.doorOpening && state.doorOpenAmount > 0.0f)
        state.doorOpenAmount -= speed * deltaTime;
    state.doorOpenAmount = glm::clamp(state.doorOpenAmount, 0.0f, 1.0f);
    
    if (state.windowOpening && state.windowOpenAmount < 1.0f)
        state.windowOpenAmount += speed * deltaTime;
    else if (!state.windowOpening && state.windowOpenAmount > 0.0f)
        state.windowOpenAmount -= speed * deltaTime;
    state.windowOpenAmount = glm::clamp(state.windowOpenAmount, 0.0f, 1.0f);
}

void drawBarracksScene(Shader& shader, glm::mat4 parent, SceneState& state) {
    // Floor
    drawFloor(shader, parent, ROOM_WIDTH, ROOM_DEPTH);
    
    // Walls
    drawWalls(shader, parent, ROOM_WIDTH, ROOM_HEIGHT, ROOM_DEPTH);
    
    // Ceiling
    drawCeiling(shader, parent, ROOM_WIDTH, ROOM_HEIGHT, ROOM_DEPTH);
    
    // Door (back wall center)
    float doorX = (ROOM_WIDTH - 1.0f) / 2.0f;
    float doorAngle = state.doorOpenAmount * 90.0f;
    drawDoor(shader, parent, doorX, 0.0f, ROOM_DEPTH - 0.2f, doorAngle);
    
    // Window (left wall)
    drawWindow(shader, parent, 0.0f, 1.2f, ROOM_DEPTH * 0.3f, state.windowOpenAmount);
    
    // Bunk beds - Left side (3 beds)
    for (int i = 0; i < 3; i++) {
        float zPos = 0.5f + i * 3.5f;
        drawBunkBed(shader, parent, 0.3f, 0.0f, zPos);
    }
    
    // Bunk beds - Right side (3 beds)
    for (int i = 0; i < 3; i++) {
        float zPos = 0.5f + i * 3.5f;
        drawBunkBed(shader, parent, ROOM_WIDTH - 1.3f, 0.0f, zPos);
    }
    
    // Stove (center-back)
    drawStove(shader, parent, ROOM_WIDTH / 2.0f, 0.0f, ROOM_DEPTH - 2.5f);
    
    // Hanging lamps
    float lampY = ROOM_HEIGHT - 0.1f;
    drawHangingLamp(shader, parent, ROOM_WIDTH / 3.0f, lampY, ROOM_DEPTH / 3.0f);
    drawHangingLamp(shader, parent, 2.0f * ROOM_WIDTH / 3.0f, lampY, 2.0f * ROOM_DEPTH / 3.0f);
}

#endif
