//
// Created by adrian on 17/05/25.
//

#include "ResizeListener.h"

#include <iostream>

#include "core/Constants.h"

ResizeListener::ResizeListener(GLFWwindow* window, glm::mat4& projection)
: m_window(window), m_projection(projection) {}

void ResizeListener::onEvent(Event& e) {
     if (e.type == EventType::WindowResized) {
          glViewport(0, 0, e.resize.width, e.resize.height);
          m_projection = glm::perspective(glm::radians(Constants::FOV),
              static_cast<float>(e.resize.width) / static_cast<float>(e.resize.height),
              Constants::NearPlane, Constants::FarPlane);
          std::cout << "d" << std::endl;
     }
}
