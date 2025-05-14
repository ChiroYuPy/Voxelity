//
// Created by adrian on 14/05/25.
//

#include "../../include/listeners/CameraController.h"

#include "rendering/Camera.h"

CameraController::CameraController(GLFWwindow* window, Camera& camera)
    : m_camera(camera), m_window(window) {}

void CameraController::onEvent(Event& e) {
    switch (e.type) {
        case EventType::KeyPressed: {
            handleKey(e.key.code, true);
            break;
        }
        case EventType::KeyReleased: {
            handleKey(e.key.code, false);
            break;
        }
        case EventType::MouseMoved: {
            handleMouse(e.mouse.x, e.mouse.y);
            break;
        }
        default:
            break;
    }
}

void CameraController::handleKey(const int key, const bool pressed) {
    if (key == GLFW_KEY_ESCAPE && pressed) {
        m_control = false;
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_moveForward  = false;
        m_moveBackward = false;
        m_moveLeft     = false;
        m_moveRight    = false;
        m_moveUp       = false;
        m_moveDown     = false;
    } else if (key == GLFW_KEY_F && pressed) {
        m_control = true;
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwGetCursorPos(m_window, &m_lastX, &m_lastY);
    }

    if (m_control) {
        switch (key) {
            case GLFW_KEY_W: m_moveForward  = pressed; break;
            case GLFW_KEY_S: m_moveBackward = pressed; break;
            case GLFW_KEY_A: m_moveLeft     = pressed; break;
            case GLFW_KEY_D: m_moveRight    = pressed; break;
            case GLFW_KEY_SPACE: m_moveUp   = pressed; break;
            case GLFW_KEY_LEFT_SHIFT: m_moveDown = pressed; break;
            default: break;
        }
    }
}

void CameraController::handleMouse(const double xpos, const double ypos) {
    if (m_control) {
        if (m_firstMouse) {
            m_lastX = xpos;
            m_lastY = ypos;
            m_firstMouse = false;
        }

        const auto dx = static_cast<float>(xpos - m_lastX);
        const auto dy = static_cast<float>(ypos - m_lastY);

        m_lastX = xpos;
        m_lastY = ypos;

        m_camera.rotate(dx, dy);
    }
}

void CameraController::update(const float deltaTime) const {
    if (m_moveForward)  m_camera.moveForward(deltaTime);
    if (m_moveBackward) m_camera.moveBackward(deltaTime);
    if (m_moveLeft)     m_camera.moveLeft(deltaTime);
    if (m_moveRight)    m_camera.moveRight(deltaTime);
    if (m_moveUp)       m_camera.moveUp(deltaTime);
    if (m_moveDown)     m_camera.moveDown(deltaTime);
}
