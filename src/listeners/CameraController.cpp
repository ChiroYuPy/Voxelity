//
// Created by adrian on 14/05/25.
//

#include "../../include/listeners/CameraController.h"

#include "rendering/Camera.h"

CameraController::CameraController(Camera& camera)
    : m_camera(camera) {}

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

void CameraController::handleMouse(const double xpos, const double ypos) {
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

void CameraController::update(const float deltaTime) const {
    if (m_moveForward)  m_camera.moveForward(deltaTime);
    if (m_moveBackward) m_camera.moveBackward(deltaTime);
    if (m_moveLeft)     m_camera.moveLeft(deltaTime);
    if (m_moveRight)    m_camera.moveRight(deltaTime);
    if (m_moveUp)       m_camera.moveUp(deltaTime);
    if (m_moveDown)     m_camera.moveDown(deltaTime);
}
