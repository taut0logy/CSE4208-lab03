//
//  main.cpp
//  3D Laboratory - Assignment A1
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "shader.h"
#include "camera.h"
#include "cube.h"
#include "lab_scene.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void printKeyHints();

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// Mouse state
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

Camera camera(glm::vec3(LAB_WIDTH / 2.0f, 12.0f, LAB_DEPTH / 2.0f), 
              glm::vec3(LAB_WIDTH / 2.0f, 0.0f, LAB_DEPTH / 2.0f));

LabState labState;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool keyGPressed = false;
bool keyLPressed = false;
bool keyOPressed = false;
bool keyPPressed = false;
bool keyBPressed = false;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Laboratory - CSE 4208", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader labShader("vertexShader.vs", "fragmentShader.fs");
    
    initCubeBuffers();
    
    // Start view, from back of the room, birds eye view
    camera.position = glm::vec3(LAB_WIDTH + 3.0f, 8.0f, LAB_DEPTH + 5.0f);
    camera.target = glm::vec3(LAB_WIDTH / 2.0f, 1.0f, LAB_DEPTH / 2.0f);
    camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera.orbitRadius = glm::length(camera.position - camera.target);
    
    printKeyHints();

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        updateLabState(labState, deltaTime);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(60.0f), 
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 
                                                0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        
        labShader.use();
        labShader.setMat4("projection", projection);
        labShader.setMat4("view", view);

        glm::mat4 identity = glm::mat4(1.0f);
        drawLabScene(labShader, identity, labState);

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

    // Movement (WASD + E/R)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.moveUp(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        camera.moveDown(deltaTime);

    // Rotation (X/Y/Z)
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.addPitch(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        camera.addYaw(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        camera.addRoll(deltaTime);

    // Orbit (F)
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        camera.orbit(deltaTime);

    // Bird's eye view (B) - toggle
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        if (!keyBPressed) {
            camera.setBirdsEyeView(LAB_WIDTH / 2.0f, LAB_DEPTH / 2.0f);
            keyBPressed = true;
        }
    } else {
        keyBPressed = false;
    }

    // Fan toggle (G)
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        if (!keyGPressed) {
            labState.fanOn = !labState.fanOn;
            keyGPressed = true;
        }
    } else {
        keyGPressed = false;
    }

    // Light toggle (L)
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (!keyLPressed) {
            labState.lightOn = !labState.lightOn;
            keyLPressed = true;
        }
    } else {
        keyLPressed = false;
    }

    // Door toggle (O)
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        if (!keyOPressed) {
            labState.doorOpening = !labState.doorOpening;
            keyOPressed = true;
        }
    } else {
        keyOPressed = false;
    }

    // Window toggle (P)
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (!keyPPressed) {
            labState.windowOpening = !labState.windowOpening;
            keyPPressed = true;
        }
    } else {
        keyPPressed = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

void printKeyHints() {
    cout << "\n========================================" << endl;
    cout << "    3D LABORATORY - CONTROLS" << endl;
    cout << "========================================" << endl;
    cout << "\n--- MOUSE ---" << endl;
    cout << "  Move    : Look around" << endl;
    cout << "  Scroll  : Move forward/backward" << endl;
    cout << "\n--- KEYBOARD MOVEMENT ---" << endl;
    cout << "  W/S     : Move forward/backward" << endl;
    cout << "  A/D     : Move left/right" << endl;
    cout << "  E/R     : Move up/down" << endl;
    cout << "\n--- ROTATION ---" << endl;
    cout << "  X       : Pitch (look up/down)" << endl;
    cout << "  Y       : Yaw (look left/right)" << endl;
    cout << "  Z       : Roll" << endl;
    cout << "\n--- CAMERA ---" << endl;
    cout << "  F       : Orbit around lab center" << endl;
    cout << "  B       : Bird's eye view" << endl;
    cout << "\n--- INTERACTIONS ---" << endl;
    cout << "  G       : Toggle fan ON/OFF" << endl;
    cout << "  L       : Toggle lights ON/OFF" << endl;
    cout << "  O       : Open/Close door" << endl;
    cout << "  P       : Open/Close windows" << endl;
    cout << "\n  ESC     : Exit" << endl;
    cout << "========================================\n" << endl;
}