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
    void subscribe(EventListener* listener);

    void dispatch(Event& event) const;

private:
    std::vector<EventListener*> listeners;
};

#endif //EVENTDISPATCHER_H
