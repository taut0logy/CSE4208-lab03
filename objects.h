#ifndef OBJECTS_H
#define OBJECTS_H

#include "cube.h"
#include "shader.h"
#include <glm/glm.hpp>

// Colors
const glm::vec3 COLOR_FLOOR = glm::vec3(0.75f, 0.75f, 0.78f);
const glm::vec3 COLOR_WALL = glm::vec3(0.95f, 0.93f, 0.88f);
const glm::vec3 COLOR_CEILING = glm::vec3(0.98f, 0.98f, 0.98f);
const glm::vec3 COLOR_DESK = glm::vec3(0.76f, 0.60f, 0.42f);
const glm::vec3 COLOR_CHAIR = glm::vec3(0.30f, 0.30f, 0.35f);
const glm::vec3 COLOR_MONITOR_BEZEL = glm::vec3(0.10f, 0.10f, 0.10f);
const glm::vec3 COLOR_MONITOR_SCREEN = glm::vec3(0.20f, 0.40f, 0.60f);
const glm::vec3 COLOR_DOOR = glm::vec3(0.55f, 0.35f, 0.20f);
const glm::vec3 COLOR_WINDOW_FRAME = glm::vec3(0.60f, 0.60f, 0.65f);
const glm::vec3 COLOR_WINDOW_GLASS = glm::vec3(0.70f, 0.85f, 0.95f);
const glm::vec3 COLOR_WHITEBOARD = glm::vec3(0.98f, 0.98f, 0.98f);
const glm::vec3 COLOR_WHITEBOARD_FRAME = glm::vec3(0.5f, 0.5f, 0.55f);
const glm::vec3 COLOR_FAN_BLADE = glm::vec3(0.90f, 0.90f, 0.90f);
const glm::vec3 COLOR_FAN_MOTOR = glm::vec3(0.30f, 0.30f, 0.30f);
const glm::vec3 COLOR_LIGHT_ON = glm::vec3(1.0f, 0.95f, 0.8f);
const glm::vec3 COLOR_LIGHT_OFF = glm::vec3(0.3f, 0.3f, 0.3f);

void drawDesk(Shader& shader, glm::mat4 parent, float x, float y, float z) {
    // Tabletop
    drawCuboid(shader, parent, x - 0.6f, y + 0.7f, z - 0.3f, 1.2f, 0.05f, 0.6f, COLOR_DESK);
    // Legs
    drawCuboid(shader, parent, x - 0.55f, y, z - 0.25f, 0.05f, 0.7f, 0.05f, COLOR_DESK);
    drawCuboid(shader, parent, x + 0.5f, y, z - 0.25f, 0.05f, 0.7f, 0.05f, COLOR_DESK);
    drawCuboid(shader, parent, x - 0.55f, y, z + 0.2f, 0.05f, 0.7f, 0.05f, COLOR_DESK);
    drawCuboid(shader, parent, x + 0.5f, y, z + 0.2f, 0.05f, 0.7f, 0.05f, COLOR_DESK);
}

void drawChair(Shader& shader, glm::mat4 parent, float x, float y, float z) {
    // Seat
    drawCuboid(shader, parent, x - 0.2f, y + 0.4f, z - 0.2f, 0.4f, 0.05f, 0.4f, COLOR_CHAIR);
    // Backrest
    drawCuboid(shader, parent, x - 0.2f, y + 0.45f, z + 0.15f, 0.4f, 0.5f, 0.05f, COLOR_CHAIR);
    // Legs
    drawCuboid(shader, parent, x - 0.18f, y, z - 0.18f, 0.04f, 0.4f, 0.04f, COLOR_CHAIR);
    drawCuboid(shader, parent, x + 0.14f, y, z - 0.18f, 0.04f, 0.4f, 0.04f, COLOR_CHAIR);
    drawCuboid(shader, parent, x - 0.18f, y, z + 0.14f, 0.04f, 0.4f, 0.04f, COLOR_CHAIR);
    drawCuboid(shader, parent, x + 0.14f, y, z + 0.14f, 0.04f, 0.4f, 0.04f, COLOR_CHAIR);
}

void drawPC(Shader& shader, glm::mat4 parent, float x, float y, float z) {
    // Monitor base
    drawCuboid(shader, parent, x - 0.1f, y, z - 0.08f, 0.2f, 0.02f, 0.15f, COLOR_WINDOW_FRAME);
    // Monitor stand
    drawCuboid(shader, parent, x - 0.02f, y + 0.02f, z - 0.02f, 0.04f, 0.1f, 0.04f, COLOR_WINDOW_FRAME);
    // Monitor bezel
    drawCuboid(shader, parent, x - 0.22f, y + 0.12f, z - 0.015f, 0.44f, 0.3f, 0.03f, COLOR_MONITOR_BEZEL);
    // Screen
    drawCuboid(shader, parent, x - 0.2f, y + 0.14f, z - 0.01f, 0.4f, 0.26f, 0.02f, COLOR_MONITOR_SCREEN);
    
    // CPU Tower
    // Black case
    drawCuboid(shader, parent, x - 0.5f, y, z - 0.1f, 0.18f, 0.4f, 0.4f, COLOR_MONITOR_BEZEL);
    // Power button
    drawCuboid(shader, parent, x - 0.45f, y + 0.3f, z + 0.3f, 0.08f, 0.02f, 0.01f, glm::vec3(0.8f, 0.8f, 0.8f));
    
    // Keyboard
    drawCuboid(shader, parent, x - 0.25f, y, z + 0.15f, 0.5f, 0.02f, 0.18f, COLOR_MONITOR_BEZEL);

    drawCuboid(shader, parent, x - 0.23f, y + 0.02f, z + 0.17f, 0.46f, 0.01f, 0.14f, glm::vec3(0.2f, 0.2f, 0.2f));

    // Mouse
    drawCuboid(shader, parent, x + 0.3f, y, z + 0.18f, 0.08f, 0.03f, 0.12f, COLOR_MONITOR_BEZEL);
}

void drawFloor(Shader& shader, glm::mat4 parent, float width, float depth) {
    drawCuboid(shader, parent, 0.0f, -0.1f, 0.0f, width, 0.1f, depth, COLOR_FLOOR);
}

void drawCeiling(Shader& shader, glm::mat4 parent, float width, float depth, float height) {
    drawCuboid(shader, parent, 0.0f, height, 0.0f, width, 0.1f, depth, COLOR_CEILING);
}

void drawWalls(Shader& shader, glm::mat4 parent, float width, float height, float depth, float doorOpen) {
    float wallThickness = 0.1f;
    
    // Front wall (with whiteboard)
    drawCuboid(shader, parent, 0.0f, 0.0f, 0.0f, width, height, wallThickness, COLOR_WALL);
    
    // Back wall (with door opening in center)
    float doorWidth = 1.0f;
    float doorHeight = 2.2f;
    float sideWidth = (width - doorWidth) / 2.0f;
    // Left section
    drawCuboid(shader, parent, 0.0f, 0.0f, depth - wallThickness, sideWidth, height, wallThickness, COLOR_WALL);
    // Right section
    drawCuboid(shader, parent, sideWidth + doorWidth, 0.0f, depth - wallThickness, sideWidth, height, wallThickness, COLOR_WALL);
    // Above door
    drawCuboid(shader, parent, sideWidth, doorHeight, depth - wallThickness, doorWidth, height - doorHeight, wallThickness, COLOR_WALL);
    
    // Window params to match drawWindow calls
    float wRef1 = depth * 0.3f;
    float wRef2 = depth * 0.7f;
    float winYMin = 1.45f; // Frame starts -0.05 relative to 1.5
    float winYMax = 2.93f; // Frame ends 1.43 relative to 1.5
    
    // Left wall
    
    float l_h1_start = wRef1 - 1.25f; // 1.75
    float l_h1_end = wRef1 + 0.05f;   // 3.05
    float l_h2_start = wRef2 - 1.25f; // 5.75
    float l_h2_end = wRef2 + 0.05f;   // 7.05
    
    // Seg 1 (Start to H1)
    drawCuboid(shader, parent, 0.0f, 0.0f, 0.0f, wallThickness, height, l_h1_start, COLOR_WALL);
    // Seg 2 (Under H1)
    drawCuboid(shader, parent, 0.0f, 0.0f, l_h1_start, wallThickness, winYMin, l_h1_end - l_h1_start, COLOR_WALL);
    // Seg 3 (Over H1)
    drawCuboid(shader, parent, 0.0f, winYMax, l_h1_start, wallThickness, height - winYMax, l_h1_end - l_h1_start, COLOR_WALL);
    // Seg 4 (Between H1 and H2)
    drawCuboid(shader, parent, 0.0f, 0.0f, l_h1_end, wallThickness, height, l_h2_start - l_h1_end, COLOR_WALL);
    // Seg 5 (Under H2)
    drawCuboid(shader, parent, 0.0f, 0.0f, l_h2_start, wallThickness, winYMin, l_h2_end - l_h2_start, COLOR_WALL);
    // Seg 6 (Over H2)
    drawCuboid(shader, parent, 0.0f, winYMax, l_h2_start, wallThickness, height - winYMax, l_h2_end - l_h2_start, COLOR_WALL);
    // Seg 7 (End)
    drawCuboid(shader, parent, 0.0f, 0.0f, l_h2_end, wallThickness, height, depth - l_h2_end, COLOR_WALL);
    
    // Right wall
    
    float r_h1_start = wRef1 - 0.05f; // 2.95
    float r_h1_end = wRef1 + 1.25f;   // 4.25
    float r_h2_start = wRef2 - 0.05f; // 6.95
    float r_h2_end = wRef2 + 1.25f;   // 8.25
    float rx = width - wallThickness;
    
    // Seg 1 (Start to H1)
    drawCuboid(shader, parent, rx, 0.0f, 0.0f, wallThickness, height, r_h1_start, COLOR_WALL);
    // Seg 2 (Under H1)
    drawCuboid(shader, parent, rx, 0.0f, r_h1_start, wallThickness, winYMin, r_h1_end - r_h1_start, COLOR_WALL);
    // Seg 3 (Over H1)
    drawCuboid(shader, parent, rx, winYMax, r_h1_start, wallThickness, height - winYMax, r_h1_end - r_h1_start, COLOR_WALL);
    // Seg 4 (Between H1 and H2)
    drawCuboid(shader, parent, rx, 0.0f, r_h1_end, wallThickness, height, r_h2_start - r_h1_end, COLOR_WALL);
    // Seg 5 (Under H2)
    drawCuboid(shader, parent, rx, 0.0f, r_h2_start, wallThickness, winYMin, r_h2_end - r_h2_start, COLOR_WALL);
    // Seg 6 (Over H2)
    drawCuboid(shader, parent, rx, winYMax, r_h2_start, wallThickness, height - winYMax, r_h2_end - r_h2_start, COLOR_WALL);
    // Seg 7 (End)
    drawCuboid(shader, parent, rx, 0.0f, r_h2_end, wallThickness, height, depth - r_h2_end, COLOR_WALL);
}

void drawWhiteboard(Shader& shader, glm::mat4 parent, float x, float y, float z) {
    // Frame
    drawCuboid(shader, parent, x - 2.0f, y, z, 4.0f, 1.8f, 0.08f, COLOR_WHITEBOARD_FRAME);
    // Board surface
    drawCuboid(shader, parent, x - 1.9f, y + 0.1f, z + 0.05f, 3.8f, 1.6f, 0.05f, COLOR_WHITEBOARD);
}

void drawDoor(Shader& shader, glm::mat4 parent, float x, float y, float z, float openAngle) {
    glm::mat4 doorTrans = glm::translate(parent, glm::vec3(x, y, z));

    //open door
    doorTrans = glm::rotate(doorTrans, glm::radians(openAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    drawCuboid(shader, doorTrans, 0.0f, 0.0f, 0.0f, 1.0f, 2.2f, 0.08f, COLOR_DOOR);

    drawCuboid(shader, doorTrans, 0.85f, 1.0f, -0.05f, 0.08f, 0.05f, 0.18f, COLOR_WINDOW_FRAME); // Handle through door
}

void drawWindow(Shader& shader, glm::mat4 parent, float x, float y, float z, float openAmount, float rotationAngle) {
    glm::mat4 windowTrans = glm::translate(parent, glm::vec3(x, y, z));
    windowTrans = glm::rotate(windowTrans, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Colors
    glm::vec3 oakColor = glm::vec3(0.72f, 0.53f, 0.35f);
    glm::vec3 grillColor = glm::vec3(0.35f, 0.35f, 0.38f);
    
    // Outer frame
    drawCuboid(shader, windowTrans, -0.05f, -0.05f, -0.02f, 1.3f, 0.08f, 0.14f, COLOR_WINDOW_FRAME);
    drawCuboid(shader, windowTrans, -0.05f, 1.35f, -0.02f, 1.3f, 0.08f, 0.14f, COLOR_WINDOW_FRAME);
    drawCuboid(shader, windowTrans, -0.05f, 0.0f, -0.02f, 0.08f, 1.4f, 0.14f, COLOR_WINDOW_FRAME);
    drawCuboid(shader, windowTrans, 1.17f, 0.0f, -0.02f, 0.08f, 1.4f, 0.14f, COLOR_WINDOW_FRAME);
    
    // Fixed glass par with frame
    drawCuboid(shader, windowTrans, 0.6f, 0.03f, -0.02f, 0.55f, 0.03f, 0.04f, oakColor);
    drawCuboid(shader, windowTrans, 0.6f, 1.32f, -0.02f, 0.55f, 0.03f, 0.04f, oakColor);
    drawCuboid(shader, windowTrans, 0.6f, 0.03f, -0.02f, 0.03f, 1.32f, 0.04f, oakColor);
    drawCuboid(shader, windowTrans, 1.12f, 0.03f, -0.02f, 0.03f, 1.32f, 0.04f, oakColor);
    drawCuboid(shader, windowTrans, 0.63f, 0.06f, -0.01f, 0.49f, 1.26f, 0.02f, COLOR_WINDOW_GLASS);
    
    // Sliding glass pane (left side - moves right when opening)
    float slideOffset = openAmount * 0.55f;
    float slideX = 0.03f + slideOffset;
    
    // Sliding frame
    drawCuboid(shader, windowTrans, slideX, 0.03f, 0.02f, 0.55f, 0.03f, 0.04f, oakColor);
    drawCuboid(shader, windowTrans, slideX, 1.32f, 0.02f, 0.55f, 0.03f, 0.04f, oakColor);
    drawCuboid(shader, windowTrans, slideX, 0.03f, 0.02f, 0.03f, 1.32f, 0.04f, oakColor);
    drawCuboid(shader, windowTrans, slideX + 0.52f, 0.03f, 0.02f, 0.03f, 1.32f, 0.04f, oakColor);
    drawCuboid(shader, windowTrans, slideX + 0.03f, 0.06f, 0.03f, 0.49f, 1.26f, 0.02f, COLOR_WINDOW_GLASS);
}

void drawFan(Shader& shader, glm::mat4 parent, float x, float y, float z, float rotation) {
    // Rod (Fixed - does not rotate)
    drawCuboid(shader, parent, x - 0.03f, y, z - 0.03f, 0.06f, 0.3f, 0.06f, COLOR_FAN_MOTOR);
    
    // Rotating Assembly
    glm::mat4 fanTrans = glm::translate(parent, glm::vec3(x, y - 0.1f, z));
    fanTrans = glm::rotate(fanTrans, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Motor housing (Rotates with fan)
    for(int i=0; i<4; i++) {
        glm::mat4 motorTrans = glm::rotate(fanTrans, glm::radians(i * 45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        drawCuboid(shader, motorTrans, -0.15f, -0.05f, -0.15f, 0.3f, 0.15f, 0.3f, COLOR_FAN_MOTOR);
    }
    
    // Blades
    for (int i = 0; i < 4; i++) {
        glm::mat4 bladeTrans = glm::rotate(fanTrans, glm::radians(90.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
        drawCuboid(shader, bladeTrans, 0.0f, 0.0f, -0.08f, 0.8f, 0.02f, 0.16f, COLOR_FAN_BLADE);
    }
}

void drawLight(Shader& shader, glm::mat4 parent, float x, float y, float z, bool isOn) {
    glm::vec3 color = isOn ? COLOR_LIGHT_ON : COLOR_LIGHT_OFF;
    drawCuboid(shader, parent, x - 0.3f, y, z - 0.1f, 0.6f, 0.05f, 0.2f, color);
}

#endif
