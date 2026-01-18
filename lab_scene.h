#ifndef LAB_SCENE_H
#define LAB_SCENE_H

#include "objects.h"
#include "shader.h"
#include <glm/glm.hpp>

const float LAB_WIDTH = 12.0f;
const float LAB_DEPTH = 10.0f;
const float LAB_HEIGHT = 4.0f;

struct LabState {
    bool fanOn = false;
    float fanRotation = 0.0f;
    bool lightOn = true;
    float doorAngle = 0.0f;
    bool doorOpening = false;
    float windowOpen = 0.0f;
    bool windowOpening = false;
};

void updateLabState(LabState& state, float deltaTime) {
    // Fan rotation
    if (state.fanOn) {
        state.fanRotation += 200.0f * deltaTime;
        if (state.fanRotation > 360.0f) state.fanRotation -= 360.0f;
    }
    
    // Door animation
    if (state.doorOpening && state.doorAngle < 90.0f) {
        state.doorAngle += 60.0f * deltaTime;
        if (state.doorAngle > 90.0f) state.doorAngle = 90.0f;
    } else if (!state.doorOpening && state.doorAngle > 0.0f) {
        state.doorAngle -= 60.0f * deltaTime;
        if (state.doorAngle < 0.0f) state.doorAngle = 0.0f;
    }
    
    // Window animation
    if (state.windowOpening && state.windowOpen < 1.0f) {
        state.windowOpen += 1.5f * deltaTime;
        if (state.windowOpen > 1.0f) state.windowOpen = 1.0f;
    } else if (!state.windowOpening && state.windowOpen > 0.0f) {
        state.windowOpen -= 1.5f * deltaTime;
        if (state.windowOpen < 0.0f) state.windowOpen = 0.0f;
    }
}

void drawLabScene(Shader& shader, glm::mat4 identity, LabState& state) {
    shader.use();
    shader.setBool("lightOn", state.lightOn);
    
    // Floor and ceiling
    drawFloor(shader, identity, LAB_WIDTH, LAB_DEPTH);
    drawCeiling(shader, identity, LAB_WIDTH, LAB_DEPTH, LAB_HEIGHT);
    
    // Walls
    drawWalls(shader, identity, LAB_WIDTH, LAB_HEIGHT, LAB_DEPTH, state.doorAngle);
    
    // Whiteboard on front wall
    drawWhiteboard(shader, identity, LAB_WIDTH / 2.0f, 1.5f, 0.15f);
    
    // Door on back wall
    float doorX = (LAB_WIDTH - 1.0f) / 2.0f;
    drawDoor(shader, identity, doorX, 0.0f, LAB_DEPTH - 0.12f, state.doorAngle);
    
    // Windows on left wall (2 windows) - rotated 90 degrees
    drawWindow(shader, identity, 0.12f, 1.5f, LAB_DEPTH * 0.3f, state.windowOpen, 90.0f);
    drawWindow(shader, identity, 0.12f, 1.5f, LAB_DEPTH * 0.7f, state.windowOpen, 90.0f);
    
    // Windows on right wall (2 windows) - rotated -90 degrees to face into room
    drawWindow(shader, identity, LAB_WIDTH - 0.12f, 1.5f, LAB_DEPTH * 0.3f, state.windowOpen, -90.0f);
    drawWindow(shader, identity, LAB_WIDTH - 0.12f, 1.5f, LAB_DEPTH * 0.7f, state.windowOpen, -90.0f);
    
    // Ceiling fan
    drawFan(shader, identity, LAB_WIDTH / 2.0f, LAB_HEIGHT - 0.1f, LAB_DEPTH / 2.0f, state.fanRotation);
    
    // Lights (2 lights)
    drawLight(shader, identity, LAB_WIDTH * 0.3f, LAB_HEIGHT - 0.05f, LAB_DEPTH / 2.0f, state.lightOn);
    drawLight(shader, identity, LAB_WIDTH * 0.7f, LAB_HEIGHT - 0.05f, LAB_DEPTH / 2.0f, state.lightOn);
    
    // Desks with chairs and PCs (3 rows x 5 desks)
    float startX = 1.5f;
    float startZ = 2.0f;
    float deskSpacingX = 2.0f;
    float rowSpacingZ = 2.5f;
    
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 5; col++) {
            float x = startX + col * deskSpacingX;
            float z = startZ + row * rowSpacingZ;
            
            drawDesk(shader, identity, x, 0.0f, z);
            drawChair(shader, identity, x, 0.0f, z + 0.5f);
            drawPC(shader, identity, x, 0.75f, z - 0.15f);
        }
    }
}

#endif
