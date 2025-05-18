//
// Created by adrian on 17/05/25.
//

#include "ResizeListener.h"

#include "rendering/camera/CameraProjection.h"

ResizeListener::ResizeListener(GLFWwindow* window, CameraProjection& projection)
: m_window(window), m_projection(projection) {}

void ResizeListener::onEvent(Event& e) {
     if (e.type == EventType::WindowResized) {
          glViewport(0, 0, e.resizeEvent.width, e.resizeEvent.height);
          m_projection.setAspectRatio(e.resizeEvent.width, e.resizeEvent.height);
     }
}
