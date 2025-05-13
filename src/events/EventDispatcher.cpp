//
// Created by adrian on 14/05/25.
//

#include "events/EventDispatcher.h"

void EventDispatcher::subscribe(EventListener* listener) {
    listeners.push_back(listener);
}

void EventDispatcher::dispatch(Event& event) const {
    for (auto* listener : listeners) {
        listener->onEvent(event);
        if (event.handled) break;
    }
}