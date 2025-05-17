//
// Created by adrian on 14/05/25.
//

#ifndef GLFWEVENTADAPTER_H
#define GLFWEVENTADAPTER_H

#include "EventDispatcher.h"

class GLFWEventAdapter {
public:
    explicit GLFWEventAdapter(GLFWwindow* window, EventDispatcher& dispatcher);

    static void registerCallbacks(GLFWwindow* window);

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

private:
    static EventDispatcher* s_dispatcher;
};

#endif //GLFWEVENTADAPTER_H