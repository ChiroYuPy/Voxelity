//
// Created by adrian on 14/05/25.
//

#ifndef EVENT_H
#define EVENT_H

#include "EventType.h"

#include <GLT.h>

struct Event {
    EventType type;
    bool handled = false;

    union {
        struct { int code; int scancode; int mods; } keyEvent;
        struct { unsigned int codepoint; } charEvent;
        struct { double xpos; double ypos; } mouseMoveEvent;
        struct { int button; int action; int mods; } mouseButtonEvent;
        struct { double xoffset; double yoffset; } mouseScrollEvent;
        struct { int width; int height; } resizeEvent;
        struct { int focused; } windowFocusEvent;
    };


    static Event KeyPressed(const int key, const int scancode, const int mods) {
        Event e{};
        e.type = EventType::KeyPressed;
        e.keyEvent = {key, scancode, mods};
        return e;
    }

    static Event KeyReleased(const int key, const int scancode, const int mods) {
        Event e{};
        e.type = EventType::KeyReleased;
        e.keyEvent = {key, scancode, mods};
        return e;
    }

    static Event MouseMoved(const double xpos, const double ypos) {
        Event e{};
        e.type = EventType::MouseMoved;
        e.mouseMoveEvent = {xpos, ypos};
        return e;
    }

    static Event MouseButtonPressed(const int button, const int mods) {
        Event e{};
        e.type = EventType::MouseButtonPressed;
        e.mouseButtonEvent = {button, GLFW_PRESS, mods};
        return e;
    }

    static Event MouseButtonReleased(const int button, const int mods) {
        Event e{};
        e.type = EventType::MouseButtonReleased;
        e.mouseButtonEvent = {button, GLFW_RELEASE, mods};
        return e;
    }

    static Event WindowResized(const int width, const int height) {
        Event e{};
        e.type = EventType::WindowResized;
        e.resizeEvent = {width, height};
        return e;
    }
};

#endif //EVENT_H
