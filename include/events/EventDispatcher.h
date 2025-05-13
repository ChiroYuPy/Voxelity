//
// Created by adrian on 14/05/25.
//

#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <vector>

#include "Event.h"
#include "EventListener.h"

class EventDispatcher {
public:
    void subscribe(EventListener* listener) {
        listeners.push_back(listener);
    }

    void dispatch(Event& event) const {
        for (auto* listener : listeners) {
            listener->onEvent(event);
            if (event.handled) break;
        }
    }

private:
    std::vector<EventListener*> listeners;
};

#endif //EVENTDISPATCHER_H
