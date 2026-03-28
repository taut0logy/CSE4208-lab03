#ifndef LAB_SCENE_H
#define LAB_SCENE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include "shader.h"
#include "objects.h"

// -----------------------------------------------------------------------
// Room dimensions
// -----------------------------------------------------------------------
const float ROOM_WIDTH  = 8.0f;
const float ROOM_HEIGHT = 3.5f;
const float ROOM_DEPTH  = 12.0f;

// Hanging lamp positions (exposed so main.cpp can place point lights there)
const glm::vec3 LAMP1_POS = glm::vec3(ROOM_WIDTH*0.33f, ROOM_HEIGHT-0.30f, ROOM_DEPTH*0.28f);
const glm::vec3 LAMP2_POS = glm::vec3(ROOM_WIDTH*0.67f, ROOM_HEIGHT-0.30f, ROOM_DEPTH*0.70f);

// Stove mouth position (spot light origin)
//  The stove sits at x=(W-1.5), z=(D-2.0); mouth is on +Z face at waist height
const glm::vec3 STOVE_X_CENTER = glm::vec3(ROOM_WIDTH-1.5f, 0.0f, ROOM_DEPTH-2.0f);
const glm::vec3 STOVE_MOUTH_POS = glm::vec3(
    ROOM_WIDTH-1.5f,          // x center of stove
    0.20f,                    // mouth height
    ROOM_DEPTH-2.0f+0.28f);   // +Z face of cylinder (radius=0.28)

// Street lamp head positions (spot light origins, pointing downward)
const glm::vec3 SLAMP1_POS = glm::vec3(ROOM_WIDTH*0.5f - 3.0f, 3.8f, ROOM_DEPTH + 4.0f);
const glm::vec3 SLAMP2_POS = glm::vec3(ROOM_WIDTH*0.5f + 2.5f, 3.8f, ROOM_DEPTH + 4.0f);

// -----------------------------------------------------------------------
// Scene State
// -----------------------------------------------------------------------
struct SceneState {
    bool dirLightOn     = true;
    bool pointLightsOn  = true;
    bool spotLightsOn   = true;    // controls ALL spot lights together
    bool ambientOn      = true;
    bool diffuseOn      = true;
    bool specularOn     = true;

    bool  doorOpening      = false;
    float doorOpenAmount   = 0.0f;
    bool  windowOpening    = false;
    float windowOpenAmount = 0.0f;
    bool  texturesOn       = true;
    float textureBlend     = 1.0f; // 0 = object color only, 1 = full texture color
};

void updateSceneState(SceneState& s, float dt) {
    auto step = [&](bool opening, float& amt) {
        if  (opening && amt < 1.0f) amt += 1.4f * dt;
        if (!opening && amt > 0.0f) amt -= 1.4f * dt;
        amt = glm::clamp(amt, 0.0f, 1.0f);
    };
    step(s.doorOpening,   s.doorOpenAmount);
    step(s.windowOpening, s.windowOpenAmount);
}

// -----------------------------------------------------------------------
// Draw everything
// -----------------------------------------------------------------------
void drawBarracksScene(Shader& shader, glm::mat4 parent, SceneState& state) {
    // ---- Interior ----
    drawFloor  (shader, parent, ROOM_WIDTH, ROOM_DEPTH);
    drawWalls  (shader, parent, ROOM_WIDTH, ROOM_HEIGHT, ROOM_DEPTH);
    drawCeiling(shader, parent, ROOM_WIDTH, ROOM_HEIGHT, ROOM_DEPTH);
    drawRoof   (shader, parent, ROOM_WIDTH, ROOM_HEIGHT, ROOM_DEPTH);

    // Door (back wall, centre)
    float doorX = (ROOM_WIDTH - 1.0f) * 0.5f;
    drawDoor   (shader, parent, doorX, 0.0f, ROOM_DEPTH-0.16f, state.doorOpenAmount * 90.0f);

    // Windows (left wall)
    float wZ1 = 1.2f, wZ2 = ROOM_DEPTH - 1.2f - 1.1f;
    drawWindow(shader, parent, 0.0f, 1.10f, wZ1, state.windowOpenAmount);
    drawWindow(shader, parent, 0.0f, 1.10f, wZ2, state.windowOpenAmount);

    // Bunk beds – 3 rows, both sides
    for (int i = 0; i < 3; i++) {
        float zPos = 0.4f + i * 3.7f;
        drawBunkBed(shader, parent, 0.30f,             0.0f, zPos);
        drawBunkBed(shader, parent, ROOM_WIDTH - 1.40f, 0.0f, zPos);
    }

    // Stove
    drawStove(shader, parent,
              STOVE_X_CENTER.x, STOVE_X_CENTER.y, STOVE_X_CENTER.z,
              ROOM_HEIGHT);

    // Hanging lamps
    drawHangingLamp(shader, parent, LAMP1_POS.x, LAMP1_POS.y, LAMP1_POS.z);
    drawHangingLamp(shader, parent, LAMP2_POS.x, LAMP2_POS.y, LAMP2_POS.z);

    // ---- Exterior ----
    drawExterior(shader, parent, ROOM_WIDTH, ROOM_DEPTH);

    // ---- Showcase: textured cylinder + sphere ----
    drawShowcaseObjects(shader, parent, ROOM_WIDTH, ROOM_DEPTH);

    // Street lamps (just visual objects; spot light is set up in main.cpp)
    drawStreetLamp(shader, parent, SLAMP1_POS.x - 0.55f, 0.0f, SLAMP1_POS.z - 0.20f);
    drawStreetLamp(shader, parent, SLAMP2_POS.x - 0.55f, 0.0f, SLAMP2_POS.z - 0.20f);
}

#endif
