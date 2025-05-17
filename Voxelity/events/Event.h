//
// Created by adrian on 14/05/25.
//

#ifndef EVENT_H
#define EVENT_H

#include <GLT.h>

#include "EventType.h"

struct Event {
    EventType type;
    bool handled = false;

    union {
        struct {
            int code;
            int scancode;
            int mods;
        } key{};

        struct {
            double x, y;
        } mouse;

        struct {
            int width;
            int height;
        } resize;
    };

    Event(const int k, const int s, const int a, const int m)
        : type(a == GLFW_PRESS ? EventType::KeyPressed : EventType::KeyReleased) {
        key.code = k;
        key.scancode = s;
        key.mods = m;
    }

    Event(const double xpos, const double ypos) : type(EventType::MouseMoved) {
        mouse.x = xpos;
        mouse.y = ypos;
    }

    Event(const int width, const int height) : type(EventType::WindowResized) {
        resize.width = width;
        resize.height = height;
    }
};

#endif //EVENT_H
