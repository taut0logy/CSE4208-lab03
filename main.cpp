//
//  main.cpp
//  3D Barracks - CSE 4208
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "shader.h"
#include "camera.h"
#include "cube.h"
#include "pointLight.h"
#include "lab_scene.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void printControls();

unsigned int SCR_WIDTH = 1200;
unsigned int SCR_HEIGHT = 800;

// Mouse state
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Camera
Camera camera(glm::vec3(ROOM_WIDTH / 2.0f, 2.0f, ROOM_DEPTH + 3.0f),
              glm::vec3(ROOM_WIDTH / 2.0f, 1.5f, ROOM_DEPTH / 2.0f));

// Scene state
SceneState sceneState;

// Lighting
glm::vec3 lamp1Pos = glm::vec3(ROOM_WIDTH / 3.0f, ROOM_HEIGHT - 0.3f, ROOM_DEPTH / 3.0f);
glm::vec3 lamp2Pos = glm::vec3(2.0f * ROOM_WIDTH / 3.0f, ROOM_HEIGHT - 0.3f, 2.0f * ROOM_DEPTH / 3.0f);

PointLight pointLight1(0, lamp1Pos, 
                       glm::vec3(0.3f, 0.28f, 0.2f),
                       glm::vec3(0.8f, 0.75f, 0.6f),
                       glm::vec3(1.0f, 0.95f, 0.8f),
                       1.0f, 0.09f, 0.032f);

PointLight pointLight2(1, lamp2Pos,
                       glm::vec3(0.3f, 0.28f, 0.2f),
                       glm::vec3(0.8f, 0.75f, 0.6f),
                       glm::vec3(1.0f, 0.95f, 0.8f),
                       1.0f, 0.09f, 0.032f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Key states
bool key2Pressed = false, key5Pressed = false, key6Pressed = false, key7Pressed = false;
bool keyOPressed = false, keyPPressed = false;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Barracks - CSE 4208", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader phongShader("vertexShaderPhong.vs", "fragmentShaderPhong.fs");
    
    initCubeBuffers();
    printControls();

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        updateSceneState(sceneState, deltaTime);

        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        phongShader.use();
        
        // Update light toggles
        pointLight1.setAmbient(sceneState.ambientOn);
        pointLight1.setDiffuse(sceneState.diffuseOn);
        pointLight1.setSpecular(sceneState.specularOn);
        pointLight2.setAmbient(sceneState.ambientOn);
        pointLight2.setDiffuse(sceneState.diffuseOn);
        pointLight2.setSpecular(sceneState.specularOn);
        
        if (!sceneState.pointLightsOn) {
            pointLight1.turnOff();
            pointLight2.turnOff();
        } else {
            pointLight1.turnOn();
            pointLight1.setAmbient(sceneState.ambientOn);
            pointLight1.setDiffuse(sceneState.diffuseOn);
            pointLight1.setSpecular(sceneState.specularOn);
            pointLight2.turnOn();
            pointLight2.setAmbient(sceneState.ambientOn);
            pointLight2.setDiffuse(sceneState.diffuseOn);
            pointLight2.setSpecular(sceneState.specularOn);
        }
        
        pointLight1.setUpLight(phongShader);
        pointLight2.setUpLight(phongShader);

        glm::mat4 identity = glm::mat4(1.0f);
        int halfW = SCR_WIDTH / 2;
        int halfH = SCR_HEIGHT / 2;

        // --- 4-Way Split Viewport ---

        // Top-Left: Top View
        glViewport(0, halfH, halfW, halfH);
        {
            glm::mat4 view = glm::lookAt(
                glm::vec3(ROOM_WIDTH / 2.0f, 10.0f, ROOM_DEPTH / 2.0f),
                glm::vec3(ROOM_WIDTH / 2.0f, 0.0f, ROOM_DEPTH / 2.0f),
                glm::vec3(0.0f, 0.0f, -1.0f));
            glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)halfW / halfH, 0.1f, 100.0f);
            phongShader.setMat4("view", view);
            phongShader.setMat4("projection", projection);
            phongShader.setVec3("viewPos", glm::vec3(ROOM_WIDTH / 2.0f, 10.0f, ROOM_DEPTH / 2.0f));
            drawBarracksScene(phongShader, identity, sceneState);
        }

        // Top-Right: Front View
        glViewport(halfW, halfH, halfW, halfH);
        {
            glm::mat4 view = glm::lookAt(
                glm::vec3(ROOM_WIDTH / 2.0f, 1.5f, ROOM_DEPTH + 5.0f),
                glm::vec3(ROOM_WIDTH / 2.0f, 1.5f, ROOM_DEPTH / 2.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)halfW / halfH, 0.1f, 100.0f);
            phongShader.setMat4("view", view);
            phongShader.setMat4("projection", projection);
            phongShader.setVec3("viewPos", glm::vec3(ROOM_WIDTH / 2.0f, 1.5f, ROOM_DEPTH + 5.0f));
            drawBarracksScene(phongShader, identity, sceneState);
        }

        // Bottom-Left: Left View
        glViewport(0, 0, halfW, halfH);
        {
            glm::mat4 view = glm::lookAt(
                glm::vec3(-5.0f, 1.5f, ROOM_DEPTH / 2.0f),
                glm::vec3(ROOM_WIDTH / 2.0f, 1.5f, ROOM_DEPTH / 2.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)halfW / halfH, 0.1f, 100.0f);
            phongShader.setMat4("view", view);
            phongShader.setMat4("projection", projection);
            phongShader.setVec3("viewPos", glm::vec3(-5.0f, 1.5f, ROOM_DEPTH / 2.0f));
            drawBarracksScene(phongShader, identity, sceneState);
        }

        // Bottom-Right: Perspective View (User-controlled)
        glViewport(halfW, 0, halfW, halfH);
        {
            glm::mat4 view = camera.getViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)halfW / halfH, 0.1f, 100.0f);
            phongShader.setMat4("view", view);
            phongShader.setMat4("projection", projection);
            phongShader.setVec3("viewPos", camera.position);
            drawBarracksScene(phongShader, identity, sceneState);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanupCubeBuffers();
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.moveForward(-deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.moveLeft(-deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.moveUp(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) camera.moveUp(-deltaTime);

    // Toggle point lights (2)
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if (!key2Pressed) {
            sceneState.pointLightsOn = !sceneState.pointLightsOn;
            key2Pressed = true;
        }
    } else key2Pressed = false;

    // Toggle ambient (5)
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        if (!key5Pressed) {
            sceneState.ambientOn = !sceneState.ambientOn;
            key5Pressed = true;
        }
    } else key5Pressed = false;

    // Toggle diffuse (6)
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        if (!key6Pressed) {
            sceneState.diffuseOn = !sceneState.diffuseOn;
            key6Pressed = true;
        }
    } else key6Pressed = false;

    // Toggle specular (7)
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        if (!key7Pressed) {
            sceneState.specularOn = !sceneState.specularOn;
            key7Pressed = true;
        }
    } else key7Pressed = false;

    // Toggle door (O)
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        if (!keyOPressed) {
            sceneState.doorOpening = !sceneState.doorOpening;
            keyOPressed = true;
        }
    } else keyOPressed = false;

    // Toggle window (P)
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (!keyPPressed) {
            sceneState.windowOpening = !sceneState.windowOpening;
            keyPPressed = true;
        }
    } else keyPPressed = false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void printControls() {
    cout << "\n========================================" << endl;
    cout << "    3D BARRACKS - CONTROLS" << endl;
    cout << "========================================" << endl;
    cout << "\n--- MOUSE ---" << endl;
    cout << "  Move    : Look around" << endl;
    cout << "  Scroll  : Move forward/backward" << endl;
    cout << "\n--- KEYBOARD MOVEMENT ---" << endl;
    cout << "  W/S     : Move forward/backward" << endl;
    cout << "  A/D     : Move left/right" << endl;
    cout << "  E/R     : Move up/down" << endl;
    cout << "\n--- LIGHTING TOGGLES ---" << endl;
    cout << "  2       : Toggle point lights ON/OFF" << endl;
    cout << "  5       : Toggle ambient component" << endl;
    cout << "  6       : Toggle diffuse component" << endl;
    cout << "  7       : Toggle specular component" << endl;
    cout << "\n--- SCENE CONTROLS ---" << endl;
    cout << "  O       : Open/Close Door" << endl;
    cout << "  P       : Open/Close Window" << endl;
    cout << "\n  ESC     : Exit" << endl;
    cout << "========================================\n" << endl;
}