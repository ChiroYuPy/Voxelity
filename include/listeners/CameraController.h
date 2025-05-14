//
// Created by adrian on 13/05/25.
//

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLT.h>

#include "../events/EventListener.h"

class Camera;

class CameraController final : public EventListener {
public:
    explicit CameraController(GLFWwindow* window, Camera& camera);

    void onEvent(Event& e) override;
    void update(float deltaTime) const;

private:
    Camera& m_camera;
    GLFWwindow* m_window;

    bool m_control = false;

    bool m_moveForward  = false;
    bool m_moveBackward = false;
    bool m_moveLeft     = false;
    bool m_moveRight    = false;
    bool m_moveUp       = false;
    bool m_moveDown     = false;
    bool m_sprint       = false;

    double m_lastX = 400.0, m_lastY = 300.0;
    bool m_firstMouse = true;

    void handleKey(int key, bool pressed);
    void handleMouse(double xpos, double ypos);
};

#endif //INPUTMANAGER_H
