//
// Created by adrian on 14/05/25.
//

#include "CameraController.h"

#include "../rendering/camera/Camera.h"

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
        m_sprint       = false;
    } else if (key == GLFW_KEY_F && pressed) {
        m_control = true;
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwGetCursorPos(m_window, &m_lastX, &m_lastY);
    }

    if (m_control) {
        switch (key) {
            case GLFW_KEY_W: m_moveForward       = pressed; break;
            case GLFW_KEY_S: m_moveBackward      = pressed; break;
            case GLFW_KEY_A: m_moveLeft          = pressed; break;
            case GLFW_KEY_D: m_moveRight         = pressed; break;
            case GLFW_KEY_SPACE: m_moveUp        = pressed; break;
            case GLFW_KEY_LEFT_SHIFT: m_moveDown = pressed; break;
            case GLFW_KEY_R: m_sprint  = pressed; break;
            default: break;
        }
    }
}

void CameraController::handleMouse(const double xpos, const double ypos) {
    static constexpr float mouse_sensitivity = 0.2f;

    if (m_control) {
        if (m_firstMouse) {
            m_lastX = xpos;
            m_lastY = ypos;
            m_firstMouse = false;
        }

        const auto dx = static_cast<float>(xpos - m_lastX) * mouse_sensitivity;
        const auto dy = static_cast<float>(ypos - m_lastY) * mouse_sensitivity;

        m_lastX = xpos;
        m_lastY = ypos;

        m_camera.rotate(dx, dy);
    }
}

void CameraController::update(const float deltaTime) const {
    static constexpr float walk_speed = 16.f;
    static constexpr float sprint_speed = 256.f;

    const float speed = m_sprint ? sprint_speed : walk_speed;
    const float displacement = speed * deltaTime;

    if (m_moveForward)  m_camera.moveForward(displacement);
    if (m_moveBackward) m_camera.moveBackward(displacement);
    if (m_moveLeft)     m_camera.moveLeft(displacement);
    if (m_moveRight)    m_camera.moveRight(displacement);
    if (m_moveUp)       m_camera.moveUp(displacement);
    if (m_moveDown)     m_camera.moveDown(displacement);
}
