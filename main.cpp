//
//  main.cpp  –  3D Barracks Scene (CSE 4208)
//  Phong shading with:
//    • 1 directional light (dawn sunlight)
//    • 4 point lights  (2 indoor lamps + 2 street lamp ambient glow)
//    • 4 spot lights   (stove mouth + 2 street lamp cones + dummy)
//  4-way split viewport | door/window animation | full lighting toggles
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>

#include "shader.h"
#include "camera.h"
#include "cube.h"
#include "cylinder.h"
#include "sphere.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "spotLight.h"
#include "lab_scene.h"

using namespace std;

// ---- Prototypes ----
void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);
void processInput(GLFWwindow*);
void printControls();

// ---- Window ----
unsigned int SCR_WIDTH = 1280, SCR_HEIGHT = 800;

// ---- Mouse ----
float lastX = 640.0f, lastY = 400.0f;
bool  firstMouse = true;

// ---- Camera ----
Camera camera(
    glm::vec3(ROOM_WIDTH*0.5f, 2.2f, ROOM_DEPTH+4.0f),
    glm::vec3(ROOM_WIDTH*0.5f, 1.5f, ROOM_DEPTH*0.5f));

// ---- Scene state ----
SceneState sceneState;

// ---- Lights ----

// 1. Directional: low-angle dawn light from the horizon (slightly warm)
DirectionalLight dirLight(
    glm::vec3(-0.15f, -0.20f, -0.96f),      // near-horizontal, coming from behind camera
    glm::vec3(0.08f,  0.07f,  0.06f),       // ambient
    glm::vec3(0.38f,  0.28f,  0.18f),       // diffuse (warm)
    glm::vec3(0.20f,  0.15f,  0.10f));      // specular

// 2. Point lights – 0,1: hanging lamps; 2,3: street lamp warm glow
PointLight pointLight0(0, LAMP1_POS,
    glm::vec3(0.22f, 0.20f, 0.14f),
    glm::vec3(0.90f, 0.82f, 0.60f),
    glm::vec3(1.00f, 0.95f, 0.80f),
    1.0f, 0.10f, 0.04f);

PointLight pointLight1(1, LAMP2_POS,
    glm::vec3(0.22f, 0.20f, 0.14f),
    glm::vec3(0.90f, 0.82f, 0.60f),
    glm::vec3(1.00f, 0.95f, 0.80f),
    1.0f, 0.10f, 0.04f);

// Street lamp warm ambient glow (point)
PointLight pointLight2(2, SLAMP1_POS,
    glm::vec3(0.10f, 0.09f, 0.06f),
    glm::vec3(0.70f, 0.65f, 0.45f),
    glm::vec3(0.80f, 0.76f, 0.60f),
    1.0f, 0.14f, 0.07f);

PointLight pointLight3(3, SLAMP2_POS,
    glm::vec3(0.10f, 0.09f, 0.06f),
    glm::vec3(0.70f, 0.65f, 0.45f),
    glm::vec3(0.80f, 0.76f, 0.60f),
    1.0f, 0.14f, 0.07f);

// 3. Spot lights
//   [0] = stove fire mouth – points outward (+Z) and slightly up
SpotLight spotLight0(0,
    STOVE_MOUTH_POS,
    glm::vec3(0.0f, 0.2f, 1.0f),        // outward from mouth
    glm::vec3(0.04f, 0.01f, 0.0f),      // ambient
    glm::vec3(1.00f, 0.40f, 0.02f),     // diffuse (orange fire)
    glm::vec3(1.00f, 0.60f, 0.10f),     // specular
    1.0f, 0.20f, 0.10f,
    18.0f, 28.0f);

//   [1] = street lamp 1 – points straight down from head
SpotLight spotLight1(1,
    SLAMP1_POS,
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.06f, 0.06f, 0.04f),
    glm::vec3(0.90f, 0.88f, 0.70f),
    glm::vec3(1.00f, 0.98f, 0.82f),
    1.0f, 0.10f, 0.05f,
    20.0f, 30.0f);

//   [2] = street lamp 2 – points straight down from head
SpotLight spotLight2(2,
    SLAMP2_POS,
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.06f, 0.06f, 0.04f),
    glm::vec3(0.90f, 0.88f, 0.70f),
    glm::vec3(1.00f, 0.98f, 0.82f),
    1.0f, 0.10f, 0.05f,
    20.0f, 30.0f);

//   [3] = dummy (all zeros, required since shader loops NR_SPOT_LIGHTS=4)
SpotLight spotLight3(3,
    glm::vec3(0.0f),
    glm::vec3(0.0f,-1.0f,0.0f),
    glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f),
    1.0f, 0.0f, 0.0f,
    0.0f, 0.001f);

// ---- Timing ----
float deltaTime = 0.0f, lastFrame = 0.0f;

// ---- Key debounce ----
bool k1=false, k2=false, k3=false, k5=false, k6=false, k7=false;
bool kO=false, kP=false;

// =====================================================================
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
        "3D Barracks – CSE 4208", NULL, NULL);
    if (!window) { cerr << "GLFW window failed\n"; glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "GLAD init failed\n"; return -1;
    }
    glEnable(GL_DEPTH_TEST);

    Shader shader("vertexShaderPhong.vs", "fragmentShaderPhong.fs");
    initCubeBuffers();
    initCylinderBuffers();
    initSphereBuffers();

    printControls();

    // Dawn sky clear colour (deep purple-pink)
    const glm::vec4 SKY_COLOR(0.18f, 0.12f, 0.22f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        float cur = (float)glfwGetTime();
        deltaTime = cur - lastFrame;
        lastFrame = cur;

        processInput(window);
        updateSceneState(sceneState, deltaTime);

        glClearColor(SKY_COLOR.r, SKY_COLOR.g, SKY_COLOR.b, SKY_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // Helper lambdas to apply component toggles  
        auto applyComp = [&](auto& L) {
            L.setAmbient (sceneState.ambientOn);
            L.setDiffuse (sceneState.diffuseOn);
            L.setSpecular(sceneState.specularOn);
        };

        // Directional
        if (sceneState.dirLightOn) { dirLight.turnOn(); applyComp(dirLight); }
        else                         dirLight.turnOff();
        dirLight.setUpLight(shader);

        // Point lights
        for (auto* pl : {&pointLight0, &pointLight1, &pointLight2, &pointLight3}) {
            if (sceneState.pointLightsOn) { pl->turnOn(); applyComp(*pl); }
            else                            pl->turnOff();
            pl->setUpLight(shader);
        }

        // Spot lights
        for (auto* sl : {&spotLight0, &spotLight1, &spotLight2}) {
            if (sceneState.spotLightsOn) { sl->turnOn(); applyComp(*sl); }
            else                           sl->turnOff();
            sl->setUpLight(shader);
        }
        spotLight3.turnOff();          // always-off dummy
        spotLight3.setUpLight(shader);

        // Global fill ambient
        shader.setVec3("globalAmbient",
            sceneState.ambientOn
            ? glm::vec3(0.10f, 0.08f, 0.07f)
            : glm::vec3(0.0f));

        // ---- 4-Way Split Viewport ----
        int hw = (int)SCR_WIDTH / 2, hh = (int)SCR_HEIGHT / 2;
        float aspect = (float)hw / (float)hh;
        glm::mat4 I(1.0f);

        struct VP { int x, y; glm::mat4 view; float fov; glm::vec3 eye; };
        VP ports[4] = {
            // Top-Left: overhead top-down
            {  0, hh,
               glm::lookAt(glm::vec3(ROOM_WIDTH*0.5f, 14.0f, ROOM_DEPTH*0.5f),
                           glm::vec3(ROOM_WIDTH*0.5f,  0.0f, ROOM_DEPTH*0.5f),
                           glm::vec3(0,0,-1)), 55.0f,
               glm::vec3(ROOM_WIDTH*0.5f, 14.0f, ROOM_DEPTH*0.5f) },
            // Top-Right: front view
            { hw, hh,
              glm::lookAt(glm::vec3(ROOM_WIDTH*0.5f, 2.5f, ROOM_DEPTH+7.0f),
                          glm::vec3(ROOM_WIDTH*0.5f, 1.5f, ROOM_DEPTH*0.5f),
                          glm::vec3(0,1,0)), 55.0f,
              glm::vec3(ROOM_WIDTH*0.5f, 2.5f, ROOM_DEPTH+7.0f) },
            // Bottom-Left: side view
            {  0,  0,
               glm::lookAt(glm::vec3(-7.0f, 2.5f, ROOM_DEPTH*0.5f),
                           glm::vec3(ROOM_WIDTH*0.5f, 1.5f, ROOM_DEPTH*0.5f),
                           glm::vec3(0,1,0)), 55.0f,
               glm::vec3(-7.0f, 2.5f, ROOM_DEPTH*0.5f) },
            // Bottom-Right: user perspective
            { hw,  0, camera.getViewMatrix(), 45.0f, camera.position }
        };

        for (auto& vp : ports) {
            glViewport(vp.x, vp.y, hw, hh);
            shader.setMat4("view",       vp.view);
            shader.setMat4("projection", glm::perspective(glm::radians(vp.fov), aspect, 0.1f, 150.0f));
            shader.setVec3("viewPos",    vp.eye);
            drawBarracksScene(shader, I, sceneState);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanupCubeBuffers();
    cleanupCylinderBuffers();
    cleanupSphereBuffers();
    glfwTerminate();
    return 0;
}

// =====================================================================
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.moveForward( deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.moveForward(-deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.moveLeft( deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.moveLeft(-deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.moveUp( deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) camera.moveUp(-deltaTime);

    // Debounced toggles
    auto tog = [&](int key, bool& pressed, bool& state) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            if (!pressed) { state = !state; pressed = true; }
        } else pressed = false;
    };
    tog(GLFW_KEY_1, k1, sceneState.dirLightOn);
    tog(GLFW_KEY_2, k2, sceneState.pointLightsOn);
    tog(GLFW_KEY_3, k3, sceneState.spotLightsOn);
    tog(GLFW_KEY_5, k5, sceneState.ambientOn);
    tog(GLFW_KEY_6, k6, sceneState.diffuseOn);
    tog(GLFW_KEY_7, k7, sceneState.specularOn);
    tog(GLFW_KEY_O, kO, sceneState.doorOpening);
    tog(GLFW_KEY_P, kP, sceneState.windowOpening);
}

void framebuffer_size_callback(GLFWwindow*, int w, int h) {
    SCR_WIDTH = (unsigned)w; SCR_HEIGHT = (unsigned)h;
}

void mouse_callback(GLFWwindow*, double xIn, double yIn) {
    float x = (float)xIn, y = (float)yIn;
    if (firstMouse) { lastX = x; lastY = y; firstMouse = false; }
    camera.ProcessMouseMovement(x - lastX, lastY - y);
    lastX = x; lastY = y;
}

void scroll_callback(GLFWwindow*, double, double yoffset) {
    camera.ProcessMouseScroll((float)yoffset);
}

void printControls() {
    cout << "\n==========================================\n";
    cout << "   3D BARRACKS  (CSE 4208)\n";
    cout << "==========================================\n";
    cout << "  W/S/A/D/E/R  : Camera movement\n";
    cout << "  Mouse        : Look around\n";
    cout << "  Scroll       : Forward/backward\n";
    cout << "\n  1  : Directional light (dawn sun)\n";
    cout << "  2  : Point lights (lamps + street)\n";
    cout << "  3  : Spot lights (stove + street cones)\n";
    cout << "  5  : Ambient component\n";
    cout << "  6  : Diffuse component\n";
    cout << "  7  : Specular component\n";
    cout << "\n  O  : Open / close door\n";
    cout << "  P  : Open / close windows\n";
    cout << "  ESC: Quit\n";
    cout << "==========================================\n\n";
}