//
// Created by adrian on 14/05/25.
//

#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "Event.h"

class EventListener {
public:
    virtual ~EventListener() = default;
    virtual void onEvent(Event& e) = 0;
};

#endif //EVENTLISTENER_H
