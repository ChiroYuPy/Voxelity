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
}

void GLFWEventAdapter::keyCallback(GLFWwindow*, const int key, const int scancode, const int action, const int mods) {
    if (action == GLFW_REPEAT) return;
    Event e(key, scancode, action, mods);
    s_dispatcher->dispatch(e);
}

void GLFWEventAdapter::cursorPosCallback(GLFWwindow*, const double xpos, const double ypos) {
    Event e(xpos, ypos);
    s_dispatcher->dispatch(e);
}