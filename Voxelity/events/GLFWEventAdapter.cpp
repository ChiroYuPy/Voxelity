//
// Created by adrian on 14/05/25.
//

#include "events/GLFWEventAdapter.h"

EventDispatcher* GLFWEventAdapter::s_dispatcher = nullptr;

GLFWEventAdapter::GLFWEventAdapter(GLFWwindow* window, EventDispatcher& dispatcher) {
    s_dispatcher = &dispatcher;
    registerCallbacks(window);
}

void GLFWEventAdapter::registerCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void GLFWEventAdapter::keyCallback(GLFWwindow*, const int key, const int scancode, const int action, const int mods) {
    if (!s_dispatcher || action == GLFW_REPEAT) return;

    Event e = (action == GLFW_PRESS)
        ? Event::KeyPressed(key, scancode, mods)
        : Event::KeyReleased(key, scancode, mods);

    s_dispatcher->dispatch(e);
}

void GLFWEventAdapter::cursorPosCallback(GLFWwindow*, const double xpos, const double ypos) {
    if (!s_dispatcher) return;
    Event e = Event::MouseMoved(xpos, ypos);
    s_dispatcher->dispatch(e);
}

void GLFWEventAdapter::mouseButtonCallback(GLFWwindow*, const int button, const int action, const int mods) {
    if (!s_dispatcher) return;

    Event e = (action == GLFW_PRESS)
        ? Event::MouseButtonPressed(button, mods)
        : Event::MouseButtonReleased(button, mods);

    s_dispatcher->dispatch(e);
}

void GLFWEventAdapter::framebufferSizeCallback(GLFWwindow*, const int width, const int height) {
    if (!s_dispatcher) return;
    Event e = Event::WindowResized(width, height);
    s_dispatcher->dispatch(e);
}