//
// Created by adrian on 13/05/25.
//

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <GLFW/glfw3.h>

#include "rendering/Camera.h"

class InputManager {
public:
    static void keyboard_callback(GLFWwindow* window, Camera& camera, const float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.moveForward(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.moveBackward(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.moveLeft(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.moveRight(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.moveUp(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            camera.moveDown(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    static void mouse_callback(GLFWwindow* window, const double xpos, const double ypos) {
        static float lastX = 400, lastY = 300;
        static bool firstMouse = true;

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float dx = xpos - lastX;
        float dy = ypos - lastY;
        lastX = xpos;
        lastY = ypos;

        // camera.rotate(dx, dy);
    }
};

#endif //INPUTMANAGER_H
