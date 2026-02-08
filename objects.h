#ifndef OBJECTS_H
#define OBJECTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"

// Barracks Colors
const glm::vec3 COLOR_BRICK = glm::vec3(0.55f, 0.27f, 0.07f);
const glm::vec3 COLOR_WOOD = glm::vec3(0.54f, 0.27f, 0.07f);
const glm::vec3 COLOR_WOOD_LIGHT = glm::vec3(0.72f, 0.53f, 0.35f);
const glm::vec3 COLOR_MATTRESS = glm::vec3(0.76f, 0.60f, 0.42f);
const glm::vec3 COLOR_FLOOR = glm::vec3(0.25f, 0.22f, 0.18f);
const glm::vec3 COLOR_DOOR = glm::vec3(0.4f, 0.2f, 0.1f);
const glm::vec3 COLOR_STOVE = glm::vec3(0.15f, 0.15f, 0.15f);
const glm::vec3 COLOR_LAMP = glm::vec3(0.9f, 0.85f, 0.7f);
const glm::vec3 COLOR_WINDOW_FRAME = glm::vec3(0.3f, 0.2f, 0.1f);
const glm::vec3 COLOR_GLASS = glm::vec3(0.6f, 0.7f, 0.8f);

// Draw bunk bed
void drawBunkBed(Shader& shader, glm::mat4 parent, float x, float y, float z) {
    float bedWidth = 1.0f;
    float bedDepth = 2.0f;
    float postHeight = 1.8f;
    float postThick = 0.08f;
    float lowerBedY = 0.4f;
    float upperBedY = 1.2f;
    
    // 4 Posts
    drawCuboidSimple(shader, parent, x, y, z, postThick, postHeight, postThick, COLOR_WOOD);
    drawCuboidSimple(shader, parent, x + bedWidth - postThick, y, z, postThick, postHeight, postThick, COLOR_WOOD);
    drawCuboidSimple(shader, parent, x, y, z + bedDepth - postThick, postThick, postHeight, postThick, COLOR_WOOD);
    drawCuboidSimple(shader, parent, x + bedWidth - postThick, y, z + bedDepth - postThick, postThick, postHeight, postThick, COLOR_WOOD);
    
    // Side rails (lower bed)
    drawCuboidSimple(shader, parent, x, y + lowerBedY, z, bedWidth, 0.05f, postThick, COLOR_WOOD);
    drawCuboidSimple(shader, parent, x, y + lowerBedY, z + bedDepth - postThick, bedWidth, 0.05f, postThick, COLOR_WOOD);
    
    // Side rails (upper bed)
    drawCuboidSimple(shader, parent, x, y + upperBedY, z, bedWidth, 0.05f, postThick, COLOR_WOOD);
    drawCuboidSimple(shader, parent, x, y + upperBedY, z + bedDepth - postThick, bedWidth, 0.05f, postThick, COLOR_WOOD);
    
    // Platform (lower)
    drawCuboidSimple(shader, parent, x + postThick, y + lowerBedY, z + postThick, bedWidth - 2*postThick, 0.05f, bedDepth - 2*postThick, COLOR_WOOD_LIGHT);
    // Platform (upper)
    drawCuboidSimple(shader, parent, x + postThick, y + upperBedY, z + postThick, bedWidth - 2*postThick, 0.05f, bedDepth - 2*postThick, COLOR_WOOD_LIGHT);
    
    // Mattress (lower)
    drawCuboidSimple(shader, parent, x + postThick + 0.02f, y + lowerBedY + 0.05f, z + postThick + 0.02f, 
                     bedWidth - 2*postThick - 0.04f, 0.12f, bedDepth - 2*postThick - 0.04f, COLOR_MATTRESS, 8.0f);
    // Mattress (upper)
    drawCuboidSimple(shader, parent, x + postThick + 0.02f, y + upperBedY + 0.05f, z + postThick + 0.02f,
                     bedWidth - 2*postThick - 0.04f, 0.12f, bedDepth - 2*postThick - 0.04f, COLOR_MATTRESS, 8.0f);
}

// Draw floor
void drawFloor(Shader& shader, glm::mat4 parent, float width, float depth) {
    drawCuboidSimple(shader, parent, 0.0f, -0.1f, 0.0f, width, 0.1f, depth, COLOR_FLOOR, 4.0f);
}

// Draw walls (brick style)
void drawWalls(Shader& shader, glm::mat4 parent, float width, float height, float depth) {
    float thick = 0.15f;
    
    // Front wall
    drawCuboidSimple(shader, parent, 0.0f, 0.0f, 0.0f, width, height, thick, COLOR_BRICK);
    
    // Back wall (with door hole)
    float doorWidth = 1.0f;
    float doorHeight = 2.2f;
    float sideW = (width - doorWidth) / 2.0f;
    drawCuboidSimple(shader, parent, 0.0f, 0.0f, depth - thick, sideW, height, thick, COLOR_BRICK);
    drawCuboidSimple(shader, parent, sideW + doorWidth, 0.0f, depth - thick, sideW, height, thick, COLOR_BRICK);
    drawCuboidSimple(shader, parent, sideW, doorHeight, depth - thick, doorWidth, height - doorHeight, thick, COLOR_BRICK);
    
    // Left wall (with window hole)
    float winY = 1.2f, winH = 1.0f, winZ = depth * 0.3f, winD = 1.2f;
    drawCuboidSimple(shader, parent, 0.0f, 0.0f, thick, thick, height, winZ - thick, COLOR_BRICK);
    drawCuboidSimple(shader, parent, 0.0f, 0.0f, winZ, thick, winY, winD, COLOR_BRICK);
    drawCuboidSimple(shader, parent, 0.0f, winY + winH, winZ, thick, height - winY - winH, winD, COLOR_BRICK);
    drawCuboidSimple(shader, parent, 0.0f, 0.0f, winZ + winD, thick, height, depth - thick - winZ - winD, COLOR_BRICK);
    
    // Right wall (solid)
    drawCuboidSimple(shader, parent, width - thick, 0.0f, thick, thick, height, depth - 2*thick, COLOR_BRICK);
}

// Draw sloped wooden ceiling with beams
void drawCeiling(Shader& shader, glm::mat4 parent, float width, float baseHeight, float depth) {
    float peakHeight = baseHeight + 0.8f;
    float beamThick = 0.12f;
    
    // Main ceiling panels (approximated as flat for simplicity)
    drawCuboidSimple(shader, parent, 0.0f, baseHeight, 0.0f, width, 0.08f, depth, COLOR_WOOD_LIGHT);
    
    // Ceiling beams
    int numBeams = 5;
    float spacing = depth / (numBeams + 1);
    for (int i = 1; i <= numBeams; i++) {
        drawCuboidSimple(shader, parent, 0.0f, baseHeight - beamThick, spacing * i, width, beamThick, beamThick, COLOR_WOOD);
    }
}

// Draw door with rotation
void drawDoor(Shader& shader, glm::mat4 parent, float x, float y, float z, float openAngle = 0.0f) {
    glm::mat4 doorTrans = glm::translate(parent, glm::vec3(x, y, z));
    doorTrans = glm::rotate(doorTrans, glm::radians(openAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    
    drawCuboidSimple(shader, doorTrans, 0.0f, 0.0f, 0.0f, 1.0f, 2.2f, 0.06f, COLOR_DOOR);
    // Handle
    drawCuboidSimple(shader, doorTrans, 0.85f, 1.0f, 0.06f, 0.06f, 0.04f, 0.04f, glm::vec3(0.3f));
}

// Draw window with slide animation
void drawWindow(Shader& shader, glm::mat4 parent, float x, float y, float z, float openAmount = 0.0f) {
    // Frame (fixed)
    drawCuboidSimple(shader, parent, x, y - 0.05f, z - 0.05f, 0.08f, 0.05f, 1.3f, COLOR_WINDOW_FRAME);
    drawCuboidSimple(shader, parent, x, y + 1.0f, z - 0.05f, 0.08f, 0.05f, 1.3f, COLOR_WINDOW_FRAME);
    drawCuboidSimple(shader, parent, x, y, z - 0.05f, 0.08f, 1.0f, 0.05f, COLOR_WINDOW_FRAME);
    drawCuboidSimple(shader, parent, x, y, z + 1.2f, 0.08f, 1.0f, 0.05f, COLOR_WINDOW_FRAME);
    
    // Fixed glass pane (back half)
    drawCuboidSimple(shader, parent, x + 0.02f, y, z + 0.6f, 0.02f, 1.0f, 0.55f, COLOR_GLASS, 64.0f);
    
    // Sliding glass pane (front, slides sideways along Z)
    float slideZ = openAmount * 0.55f;
    drawCuboidSimple(shader, parent, x + 0.04f, y, z + slideZ, 0.02f, 1.0f, 0.55f, COLOR_GLASS, 64.0f);
}

void drawStove(Shader& shader, glm::mat4 parent, float x, float y, float z, float ceilingHeight = 3.5f) {
    float bodyR = 0.25f;
    float bodyH = 0.6f;
    
    drawCuboid(shader, parent, x - bodyR, y, z - bodyR, bodyR * 2, bodyH, bodyR * 2,
               COLOR_STOVE * 0.3f, COLOR_STOVE, glm::vec3(0.2f), 16.0f, glm::vec3(0.0f));
    
    drawCuboid(shader, parent, x - bodyR + 0.05f, y + 0.1f, z + bodyR - 0.02f, bodyR * 2 - 0.1f, 0.15f, 0.02f,
               glm::vec3(0.1f), glm::vec3(0.2f, 0.05f, 0.0f), glm::vec3(0.0f), 1.0f, glm::vec3(0.8f, 0.3f, 0.0f));
    
    float pipeHeight = ceilingHeight - (y + bodyH);
    drawCuboidSimple(shader, parent, x - 0.06f, y + bodyH, z - 0.06f, 0.12f, pipeHeight, 0.12f, COLOR_STOVE);
}
void drawHangingLamp(Shader& shader, glm::mat4 parent, float x, float y, float z) {
    drawCuboidSimple(shader, parent, x - 0.01f, y, z - 0.01f, 0.02f, 0.5f, 0.02f, glm::vec3(0.1f));
    drawCuboidSimple(shader, parent, x - 0.15f, y - 0.1f, z - 0.15f, 0.3f, 0.1f, 0.3f, COLOR_LAMP);
    // Bulb (emissive)
    drawCuboid(shader, parent, x - 0.04f, y - 0.18f, z - 0.04f, 0.08f, 0.08f, 0.08f,
               glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f), 1.0f, glm::vec3(1.0f, 0.95f, 0.8f));
}

#endif
