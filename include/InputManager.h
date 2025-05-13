//
// Created by adrian on 13/05/25.
//

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLT.h>

class Camera;

class InputManager {
public:
    static void keyboard_callback(GLFWwindow* window, Camera& camera, float deltaTime);

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};

#endif //INPUTMANAGER_H
