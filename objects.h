#ifndef OBJECTS_H
#define OBJECTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include "shader.h"
#include "cube.h"
#include "cylinder.h"
#include "sphere.h"
#include "texture_loader.h"

// -----------------------------------------------------------------------
// Color palette
// -----------------------------------------------------------------------
const glm::vec3 COLOR_BRICK        = glm::vec3(0.55f, 0.27f, 0.07f);
const glm::vec3 COLOR_WOOD         = glm::vec3(0.45f, 0.22f, 0.06f);
const glm::vec3 COLOR_WOOD_LIGHT   = glm::vec3(0.68f, 0.48f, 0.28f);
const glm::vec3 COLOR_MATTRESS     = glm::vec3(0.72f, 0.56f, 0.38f);
const glm::vec3 COLOR_PILLOW       = glm::vec3(0.88f, 0.84f, 0.76f);
const glm::vec3 COLOR_FLOOR        = glm::vec3(0.30f, 0.26f, 0.20f);
const glm::vec3 COLOR_DOOR         = glm::vec3(0.40f, 0.20f, 0.09f);
const glm::vec3 COLOR_STOVE        = glm::vec3(0.13f, 0.13f, 0.13f);
const glm::vec3 COLOR_LAMP_SHADE   = glm::vec3(0.80f, 0.74f, 0.58f);
const glm::vec3 COLOR_LAMP_CORD    = glm::vec3(0.18f, 0.16f, 0.14f);
const glm::vec3 COLOR_WINDOW_FRAME = glm::vec3(0.32f, 0.20f, 0.10f);
const glm::vec3 COLOR_GLASS        = glm::vec3(0.50f, 0.62f, 0.80f);
const glm::vec3 COLOR_METAL        = glm::vec3(0.50f, 0.48f, 0.46f);
const glm::vec3 COLOR_ROAD         = glm::vec3(0.18f, 0.17f, 0.16f);
const glm::vec3 COLOR_SOIL         = glm::vec3(0.32f, 0.22f, 0.12f);
const glm::vec3 COLOR_ROAD_LINE    = glm::vec3(0.88f, 0.82f, 0.40f);
const glm::vec3 COLOR_POLE         = glm::vec3(0.38f, 0.37f, 0.36f);
const glm::vec3 COLOR_SKY_DAWN     = glm::vec3(0.50f, 0.32f, 0.48f);
const glm::vec3 COLOR_HORIZON      = glm::vec3(0.92f, 0.55f, 0.28f);

// -----------------------------------------------------------------------
// TEXTURE IDs  (loaded once)
// -----------------------------------------------------------------------
unsigned int texBrick  = 0;
unsigned int texWood   = 0;
unsigned int texGround = 0;
unsigned int texRoad   = 0;
unsigned int texCloth  = 0;
unsigned int texCloth2 = 0;
unsigned int texFloor  = 0;

void initTextures() {
    texBrick  = loadTexture("textures/brick.jpg");
    texWood   = loadTexture("textures/wood.jpg");
    texGround = loadTexture("textures/ground.jpg");
    texRoad   = loadTexture("textures/road.jpg");
    texCloth  = loadTexture("textures/cloth.jpg");
    texCloth2 = loadTexture("textures/cloth-2.jpg");
    texFloor  = loadTexture("textures/floor.jpg");
}

// Global flag for texture toggle (updated from main)
extern bool texturesEnabled; 

// Bind a texture and enable it in shader
void bindTex(Shader& shader, unsigned int texID, float repeat = 1.0f) {
    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    shader.setInt("texture1", 0);
    shader.setBool("useTexture", texturesEnabled);
    shader.setFloat("texRepeat", repeat);
}

// Disable texture for subsequent draws
void unbindTex(Shader& shader) {
    shader.use();
    shader.setBool("useTexture", false);
}

// -----------------------------------------------------------------------
// INTERIOR: Bunk Bed  (all cubes)
// -----------------------------------------------------------------------
void drawBunkBed(Shader& shader, glm::mat4 parent, float x, float y, float z) {
    const float bW  = 1.1f;
    const float bD  = 2.2f;
    const float pT  = 0.09f;   // post thickness
    const float pH  = 2.0f;   // post height
    const float lowerBoard = 0.45f;
    const float upperBoard = 1.30f;
    const float railH      = 0.06f;
    const float mattH      = 0.14f;
    const float pillH      = 0.11f;

    // --- 4 corner posts --- (wood texture)
    bindTex(shader, texWood, 1.0f);
    for (float px : {x, x + bW - pT})
        for (float pz : {z, z + bD - pT})
            drawCuboidSimple(shader, parent, px, y, pz, pT, pH, pT, COLOR_WOOD);

    // --- headboard/footboard slats ---
    drawCuboidSimple(shader, parent, x, lowerBoard - railH, z, bW, railH, pT, COLOR_WOOD);
    drawCuboidSimple(shader, parent, x, upperBoard - railH, z, bW, railH, pT, COLOR_WOOD);
    drawCuboidSimple(shader, parent, x, lowerBoard - railH, z+bD-pT, bW, railH, pT, COLOR_WOOD);
    drawCuboidSimple(shader, parent, x, upperBoard - railH, z+bD-pT, bW, railH, pT, COLOR_WOOD);

    // --- bed boards (platform) ---
    float pW = bW - 2*pT, pDlen = bD - 2*pT;
    drawCuboidSimple(shader, parent, x+pT, lowerBoard - 0.05f, z+pT, pW, 0.05f, pDlen, COLOR_WOOD_LIGHT);
    drawCuboidSimple(shader, parent, x+pT, upperBoard - 0.05f, z+pT, pW, 0.05f, pDlen, COLOR_WOOD_LIGHT);

    // --- mattresses --- (cloth texture)
    float mW = pW - 0.04f, mDlen = pDlen - 0.04f;
    bindTex(shader, texCloth, 2.0f);
    drawCuboidSimple(shader, parent, x+pT+0.02f, lowerBoard,         z+pT+0.02f, mW, mattH, mDlen, COLOR_MATTRESS, 8.0f);
    drawCuboidSimple(shader, parent, x+pT+0.02f, upperBoard,         z+pT+0.02f, mW, mattH, mDlen, COLOR_MATTRESS, 8.0f);
    unbindTex(shader);

    // --- pillows (flat cuboids with cloth-2 texture) ---
    float pillTopL = lowerBoard + mattH;
    float pillTopU = upperBoard + mattH;
    float pillW = 0.32f, pillD = 0.22f;
    float pilX  = x + pT + 0.06f;
    float pilZ  = z + pT + 0.04f;
    bindTex(shader, texCloth2, 1.0f);
    drawCuboidSimple(shader, parent, pilX, pillTopL, pilZ,
                     pillW, pillH, pillD, COLOR_PILLOW, 8.0f);
    drawCuboidSimple(shader, parent, pilX, pillTopU, pilZ,
                     pillW, pillH, pillD, COLOR_PILLOW, 8.0f);
    unbindTex(shader);

    // --- upper safety rail ---
    bindTex(shader, texWood, 1.0f);
    float railBot = upperBoard + mattH + 0.04f;
    drawCuboidSimple(shader, parent, x, railBot, z+bD*0.5f, bW, 0.04f, pT, COLOR_WOOD);
    unbindTex(shader);
}

// -----------------------------------------------------------------------
// Floor
// -----------------------------------------------------------------------
void drawFloor(Shader& shader, glm::mat4 parent, float width, float depth) {
    bindTex(shader, texFloor, 4.0f);
    drawCuboidSimple(shader, parent, 0.0f, 0.01f, 0.0f, width, 0.06f, depth, COLOR_FLOOR, 4.0f);
    unbindTex(shader);
}

// -----------------------------------------------------------------------
// Walls
// -----------------------------------------------------------------------
void drawWalls(Shader& shader, glm::mat4 parent,
               float width, float height, float depth) {
    const float T = 0.15f;

    bindTex(shader, texBrick, 4.0f);

    // Front wall (solid)
    drawCuboidSimple(shader, parent, 0.0f, 0.0f, 0.0f, width, height, T, COLOR_BRICK);
    // Back wall with door hole
    const float dW = 1.0f, dH = 2.35f;
    float sW = (width - dW) * 0.5f;
    drawCuboidSimple(shader, parent, 0.0f,  0.0f, depth-T, sW,         height,       T, COLOR_BRICK);
    drawCuboidSimple(shader, parent, sW+dW, 0.0f, depth-T, sW,         height,       T, COLOR_BRICK);
    drawCuboidSimple(shader, parent, sW,    dH,   depth-T, dW, height-dH,             T, COLOR_BRICK);
    // Left wall – two windows
    float wH=1.1f, wY=1.1f, wD=1.1f, wZ1=1.2f;
    float wZ2 = depth - 1.2f - wD;
    drawCuboidSimple(shader, parent, 0.0f, 0.0f, T,       T, height, wZ1-T,               COLOR_BRICK);
    drawCuboidSimple(shader, parent, 0.0f, 0.0f, wZ1,     T, wY,     wD,                  COLOR_BRICK);
    drawCuboidSimple(shader, parent, 0.0f, wY+wH, wZ1,    T, height-wY-wH, wD,            COLOR_BRICK);
    float mid1 = wZ1+wD;
    drawCuboidSimple(shader, parent, 0.0f, 0.0f, mid1,    T, height, wZ2-mid1,             COLOR_BRICK);
    drawCuboidSimple(shader, parent, 0.0f, 0.0f, wZ2,     T, wY,     wD,                  COLOR_BRICK);
    drawCuboidSimple(shader, parent, 0.0f, wY+wH, wZ2,    T, height-wY-wH, wD,            COLOR_BRICK);
    drawCuboidSimple(shader, parent, 0.0f, 0.0f, wZ2+wD,  T, height, (depth-T)-(wZ2+wD),  COLOR_BRICK);
    // Right wall (solid)
    drawCuboidSimple(shader, parent, width-T, 0.0f, T, T, height, depth-2*T, COLOR_BRICK);

    unbindTex(shader);
}

// -----------------------------------------------------------------------
// Ceiling with beams
// -----------------------------------------------------------------------
void drawCeiling(Shader& shader, glm::mat4 parent,
                 float width, float height, float depth) {
    bindTex(shader, texWood, 3.0f);
    drawCuboidSimple(shader, parent, 0.0f, height, 0.0f, width, 0.09f, depth, COLOR_WOOD_LIGHT);
    const float bT = 0.14f;
    int n = 6;
    float gap = depth / (n+1);
    for (int i = 1; i <= n; i++)
        drawCuboidSimple(shader, parent, 0.0f, height-bT, gap*i, width, bT, bT, COLOR_WOOD);
    unbindTex(shader);
}

// -----------------------------------------------------------------------
// Gabled Pitched Roof  (two rotated panels + front & back gable walls)
// -----------------------------------------------------------------------
void drawRoof(Shader& shader, glm::mat4 parent,
              float width, float wallH, float depth) {
    const float ridgeH   = 1.6f;
    const float overhang = 0.40f;
    const float thick    = 0.18f;
    const float halfRun  = width * 0.5f + overhang;
    float slant = sqrtf(halfRun * halfRun + ridgeH * ridgeH);
    float angle = atan2f(ridgeH, halfRun);

    float roofDepth = depth + 2.0f * overhang;

    // ---- Roof panels (wood texture) ----
    bindTex(shader, texWood, 4.0f);

    // Left panel
    {
        glm::mat4 m = glm::translate(parent, glm::vec3(-overhang, wallH, -overhang));
        m = glm::rotate(m, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        drawCuboidSimple(shader, m, 0.0f, 0.0f, 0.0f,
                         slant, thick, roofDepth, COLOR_WOOD);
        drawCuboidSimple(shader, m, 0.0f, -0.03f, 0.0f,
                         slant, 0.03f, roofDepth, COLOR_WOOD * 0.55f);
    }
    // Right panel
    {
        glm::mat4 m = glm::translate(parent, glm::vec3(width + overhang, wallH, -overhang));
        m = glm::rotate(m, glm::pi<float>() - angle, glm::vec3(0.0f, 0.0f, 1.0f));
        drawCuboidSimple(shader, m, 0.0f, 0.0f, 0.0f,
                         slant, thick, roofDepth, COLOR_WOOD);
        drawCuboidSimple(shader, m, 0.0f, -0.03f, 0.0f,
                         slant, 0.03f, roofDepth, COLOR_WOOD * 0.55f);
    }

    // Ridge cap
    drawCuboidSimple(shader, parent,
                     width * 0.5f - 0.10f, wallH + ridgeH - thick * 0.3f, -overhang,
                     0.20f, thick * 0.6f, roofDepth, COLOR_WOOD_LIGHT);
    unbindTex(shader);

    // ---- Gable walls (brick texture) ----
    bindTex(shader, texBrick, 3.0f);
    const float T      = 0.15f;
    const int   steps  = 30;
    float sliceH = ridgeH / (float)steps;

    for (int i = 0; i < steps; i++) {
        float baseY  = wallH + i * sliceH;
        float frac   = (float)i / (float)steps;
        float sliceW = width * (1.0f - frac);
        float sliceX = (width - sliceW) * 0.5f;

        // Back gable
        drawCuboidSimple(shader, parent,
                         sliceX, baseY, depth - T,
                         sliceW, sliceH, T, COLOR_BRICK);
        // Front gable
        drawCuboidSimple(shader, parent,
                         sliceX, baseY, 0.0f,
                         sliceW, sliceH, T, COLOR_BRICK);
    }
    unbindTex(shader);
}

// -----------------------------------------------------------------------
// Door  (rotates on Y-axis hinge at x-edge)
// -----------------------------------------------------------------------
void drawDoor(Shader& shader, glm::mat4 parent,
              float x, float y, float z, float openAngle = 0.0f) {
    glm::mat4 P = glm::translate(parent, glm::vec3(x, y, z));
    P = glm::rotate(P, glm::radians(openAngle), glm::vec3(0,1,0));

    bindTex(shader, texWood, 2.0f);
    // Panel
    drawCuboidSimple(shader, P, 0.0f, 0.0f, 0.0f, 1.0f, 2.35f, 0.07f, COLOR_DOOR);
    // Inset panels
    drawCuboidSimple(shader, P, 0.06f, 0.10f, 0.07f, 0.38f, 0.85f, 0.02f, COLOR_DOOR*0.75f);
    drawCuboidSimple(shader, P, 0.56f, 0.10f, 0.07f, 0.38f, 0.85f, 0.02f, COLOR_DOOR*0.75f);
    drawCuboidSimple(shader, P, 0.06f, 1.08f, 0.07f, 0.88f, 0.90f, 0.02f, COLOR_DOOR*0.75f);
    unbindTex(shader);

    // Handle (sphere – no texture)
    drawSphereSimple(shader, P, 0.83f, 1.10f, 0.05f, 0.08f, 0.08f, 0.04f, COLOR_METAL, 64.0f);
}

// -----------------------------------------------------------------------
// Window  (slides sideways when opened)
// -----------------------------------------------------------------------
void drawWindow(Shader& shader, glm::mat4 parent,
                float x, float y, float z, float openAmt = 0.0f) {
    float fw = 0.09f;
    // Window frame – no texture (small pieces)
    drawCuboidSimple(shader, parent, x, y-fw,    z-fw,   fw, fw,   1.20f, COLOR_WINDOW_FRAME);
    drawCuboidSimple(shader, parent, x, y+1.10f, z-fw,   fw, fw,   1.20f, COLOR_WINDOW_FRAME);
    drawCuboidSimple(shader, parent, x, y,       z-fw,   fw, 1.10f, fw,   COLOR_WINDOW_FRAME);
    drawCuboidSimple(shader, parent, x, y,       z+1.10f,fw, 1.10f, fw,   COLOR_WINDOW_FRAME);
    // Fixed pane
    drawCuboidSimple(shader, parent, x+0.02f, y, z+0.55f, 0.02f, 1.10f, 0.50f, COLOR_GLASS, 64.0f);
    // Sliding pane
    float slide = openAmt * 0.50f;
    drawCuboidSimple(shader, parent, x+0.04f, y, z+slide, 0.02f, 1.10f, 0.50f, COLOR_GLASS, 64.0f);
}

// -----------------------------------------------------------------------
// Stove  (cylinder body, cylinder chimney, emissive fire mouth)
// -----------------------------------------------------------------------
void drawStove(Shader& shader, glm::mat4 parent,
               float x, float y, float z, float ceilH = 3.5f) {
    const float bR = 0.28f, bH = 0.65f;

    // Iron cylinder body (no texture – it's metal)
    drawCylinder(shader, parent, x-bR, y, z-bR,
                 bR*2, bH, bR*2,
                 COLOR_STOVE*0.35f, COLOR_STOVE, glm::vec3(0.4f), 32.0f);

    // Emissive fire-mouth slot
    glm::vec3 fireAmb (0.08f, 0.03f, 0.0f);
    glm::vec3 fireDiff(0.15f, 0.06f, 0.0f);
    glm::vec3 fireEmis(1.0f,  0.40f, 0.02f);
    drawCuboid(shader, parent,
               x-bR*0.5f, y+0.10f, z+bR-0.03f,
               bR, 0.20f, 0.03f,
               fireAmb, fireDiff, glm::vec3(0.0f), 1.0f, fireEmis);

    // Chimney pipe
    float pipeH = ceilH - (y + bH) + 0.05f;
    drawCylinder(shader, parent, x-0.07f, y+bH, z-0.07f,
                 0.14f, pipeH, 0.14f,
                 COLOR_STOVE*0.35f, COLOR_STOVE, glm::vec3(0.3f), 16.0f);

    // Four small sphere feet
    for (float fx : {x - bR*0.75f, x + bR*0.75f - 0.08f})
        for (float fz : {z - bR*0.75f, z + bR*0.75f - 0.08f})
            drawSphereSimple(shader, parent, fx, y-0.06f, fz, 0.08f,0.07f,0.08f, COLOR_METAL, 32.0f);
}

// -----------------------------------------------------------------------
// Hanging lamp
// -----------------------------------------------------------------------
void drawHangingLamp(Shader& shader, glm::mat4 parent,
                     float x, float y, float z) {
    // Cord
    drawCuboidSimple(shader, parent, x-0.01f, y, z-0.01f, 0.02f, 0.60f, 0.02f, COLOR_LAMP_CORD);
    // Shade
    drawCylinderSimple(shader, parent, x-0.18f, y-0.18f, z-0.18f, 0.36f,0.16f,0.36f, COLOR_LAMP_SHADE);
    // Emissive bulb sphere
    drawSphere(shader, parent, x-0.07f, y-0.30f, z-0.07f,
               0.14f, 0.14f, 0.14f,
               glm::vec3(1.0f), glm::vec3(1.0f,0.95f,0.80f), glm::vec3(0.0f),
               1.0f, glm::vec3(1.5f, 1.4f, 1.1f));
}

// -----------------------------------------------------------------------
// Street lamp
// -----------------------------------------------------------------------
void drawStreetLamp(Shader& shader, glm::mat4 parent,
                    float x, float y, float z) {
    float poleH = 3.8f;
    float poleR = 0.06f;

    drawCylinderSimple(shader, parent, x-poleR, y, z-poleR,
                       poleR*2, poleH, poleR*2, COLOR_POLE);

    float armL = 0.55f;
    drawCuboidSimple(shader, parent, x, y+poleH, z-0.04f,
                     armL, 0.07f, 0.07f, COLOR_POLE);

    float headX = x + armL;
    drawCylinderSimple(shader, parent, headX-0.15f, y+poleH-0.30f, z-0.15f,
                       0.30f, 0.30f, 0.30f, COLOR_POLE);

    drawSphere(shader, parent, headX-0.10f, y+poleH-0.28f, z-0.10f,
               0.20f, 0.20f, 0.20f,
               glm::vec3(1.0f), glm::vec3(1.0f,0.95f,0.82f), glm::vec3(0.0f),
               1.0f, glm::vec3(3.0f, 2.8f, 2.2f));
}

// -----------------------------------------------------------------------
// EXTERIOR: Soil all around, Road, Road markings
// -----------------------------------------------------------------------
void drawExterior(Shader& shader, glm::mat4 parent,
                  float roomW, float roomDepth) {
    // Ground wraps all 4 sides of the barracks — very large
    const float margin = 50.0f;
    const float groundY = -0.12f;
    const float groundH = 0.12f;
    float gX = -margin;
    float gZ = -margin;
    float gW  = roomW  + 2.0f * margin;
    float gD  = roomDepth + 2.0f * margin;

    bindTex(shader, texGround, 20.0f);
    drawCuboidSimple(shader, parent, gX, groundY, gZ,
                     gW, groundH, gD, COLOR_SOIL, 2.0f);
    unbindTex(shader);

    // --- Road (raised above soil) ---
    const float roadLen = 50.0f;
    float roadW = 5.0f;
    float roadX = roomW * 0.5f - roadW * 0.5f;
    float roadY = 0.008f;
    float roadThick = 0.06f;

    bindTex(shader, texRoad, 8.0f);
    // Back road
    drawCuboidSimple(shader, parent, roadX, roadY, roomDepth,
                     roadW, roadThick, roadLen, COLOR_ROAD, 2.0f);
    // Front road
    drawCuboidSimple(shader, parent, roadX, roadY, -roadLen,
                     roadW, roadThick, roadLen, COLOR_ROAD, 2.0f);
    unbindTex(shader);

    // --- Centre dashes (both directions) ---
    float dashLen = 1.2f, dashGap = 0.8f;
    float cx = roadX + roadW * 0.5f - 0.06f;
    float dashY = roadY + roadThick + 0.001f;
    // Back dashes
    for (float dz = roomDepth + 0.5f; dz < roomDepth + roadLen; dz += dashLen + dashGap)
        drawCuboidSimple(shader, parent, cx, dashY, dz, 0.12f, 0.01f, dashLen, COLOR_ROAD_LINE, 8.0f);
    // Front dashes
    for (float dz = -roadLen + 0.5f; dz < 0.0f; dz += dashLen + dashGap)
        drawCuboidSimple(shader, parent, cx, dashY, dz, 0.12f, 0.01f, dashLen, COLOR_ROAD_LINE, 8.0f);
}

void drawShowcaseObjects(Shader& shader, glm::mat4 parent,
                         float roomW, float roomDepth) {
    // --- Brick-textured cylinder (decorative column/barrel) ---
    //   Placed in the centre-right area of the room, on the floor
    float cylX = roomW * 0.5f - 0.3f;
    float cylZ = roomDepth * 0.5f;
    bindTex(shader, texBrick, 2.0f);
    drawCylinderSimple(shader, parent,
                       cylX, 0.07f, cylZ,
                       0.6f, 1.0f, 0.6f,
                       COLOR_BRICK, 16.0f);
    unbindTex(shader);

    // --- Wood-textured sphere (decorative globe) ---
    //   Sits on top of the cylinder
    bindTex(shader, texWood, 2.0f);
    drawSphereSimple(shader, parent,
                     cylX + 0.05f, 1.07f, cylZ + 0.05f,
                     0.5f, 0.5f, 0.5f,
                     COLOR_WOOD, 32.0f);
    unbindTex(shader);
}

#endif
